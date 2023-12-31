/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "dhcpserver.h"
#include "dnsserver.h"

#define TCP_PORT 80
#define DEBUG_printf printf
#define POLL_TIME_S 5
#define LOGIN_PAGE_REQUEST "/login"
#define HTTP_GET "GET"
#define HTTP_RESPONSE_HEADERS "HTTP/1.1 %d OK\nContent-Length: %d\nContent-Type: text/html; charset=utf-8\nConnection: close\n\n"
#define HTTP_RESPONSE_REDIRECT "HTTP/1.1 302 Redirect\nLocation: http://%s" LOGIN_PAGE_REQUEST "\n\n"
#define LOGIN_PAGE_HTML ""
#define TCP_RESULT_BUFFER_SIZE 2048 //buffer to store html

typedef struct TCP_SERVER_T_ {
    struct tcp_pcb *server_pcb;
    bool complete;
    ip_addr_t gw;
    async_context_t *context;
} TCP_SERVER_T;

typedef struct TCP_CONNECT_STATE_T_ {
    struct tcp_pcb *pcb;
    int sent_len;
    char headers[128];
    char result[TCP_RESULT_BUFFER_SIZE];
    int header_len;
    int result_len;
    ip_addr_t *gw;
} TCP_CONNECT_STATE_T;

// Function to close client connection
static err_t tcp_close_client_connection(TCP_CONNECT_STATE_T *con_state, struct tcp_pcb *client_pcb, err_t close_err) {
    if (client_pcb) {
        assert(con_state && con_state->pcb == client_pcb);
        tcp_arg(client_pcb, NULL);
        tcp_poll(client_pcb, NULL, 0);
        tcp_sent(client_pcb, NULL);
        tcp_recv(client_pcb, NULL);
        tcp_err(client_pcb, NULL);
        err_t err = tcp_close(client_pcb);
        if (err != ERR_OK) {
            DEBUG_printf("close failed %d, calling abort\n", err);
            tcp_abort(client_pcb);
            close_err = ERR_ABRT;
        }
        if (con_state) {
            free(con_state);
        }
    }
    return close_err;
}

// Function to close TCP server
static void tcp_server_close(TCP_SERVER_T *state) {
    if (state->server_pcb) {
        tcp_arg(state->server_pcb, NULL);
        tcp_close(state->server_pcb);
        state->server_pcb = NULL;
    }
}

// Callback function when data is successfully sent to client
static err_t tcp_server_sent(void *arg, struct tcp_pcb *pcb, u16_t len) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T*)arg;
    DEBUG_printf("tcp_server_sent %u\n", len);
    con_state->sent_len += len;

    // Add debug message to print when all data is sent
    if (con_state->sent_len >= con_state->header_len + con_state->result_len) {
        DEBUG_printf("All data sent\n");
        return tcp_close_client_connection(con_state, pcb, ERR_OK);
    }

    return ERR_OK;
}

// Function to generate HTML content for login page
static void generate_login_page_html(char *result, size_t max_result_len) {
    const char *login_page_html =
    "<html>\n"
    "<head>\n"
    "    <title>MyRepublic@Starbucks Login</title>\n"
    "    <style>\n"
    "        body {\n"
    "            font-family: 'Arial', sans-serif;\n"
    "            background-color: #f5f5f5;\n"
    "            text-align: center;\n"
    "            margin-top: 50px;\n"
    "        }\n"
    "\n"
    "        h2 {\n"
    "            color: #006633; /* Starbucks green */\n"
    "        }\n"
    "\n"
    "        form {\n"
    "            max-width: 300px;\n"
    "            margin: 0 auto;\n"
    "            background-color: #ffffff;\n"
    "            padding: 20px;\n"
    "            border-radius: 10px;\n"
    "            box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);\n"
    "        }\n"
    "\n"
    "        label {\n"
    "            display: block;\n"
    "            margin: 10px 0;\n"
    "            color: #333;\n"
    "        }\n"
    "\n"
    "        input {\n"
    "            width: 100%;\n"
    "            padding: 8px;\n"
    "            margin-bottom: 10px;\n"
    "            box-sizing: border-box;\n"
    "        }\n"
    "\n"
    "        input[type=\"submit\"] {\n"
    "            background-color: #006633;\n"
    "            color: #ffffff;\n"
    "            cursor: pointer;\n"
    "        }\n"
    "    </style>\n"
    "</head>\n"
    "<body>\n"
    "    <h2>Starbucks Login</h2>\n"
    "    <form action=\"/submit\" method=\"post\">\n"
    "        <div>\n"
    "            <label for=\"username\">Username:</label>\n"
    "            <input type=\"text\" id=\"username\" name=\"username\" required>\n"
    "        </div>\n"
    "        <div>\n"
    "            <label for=\"password\">Password:</label>\n"
    "            <input type=\"password\" id=\"password\" name=\"password\" required>\n"
    "        </div>\n"
    "        <input type=\"submit\" value=\"Login\">\n"
    "    </form>\n"
    "</body>\n"
    "</html>";

    // Function to handle different content based on request
    size_t login_page_len = strlen(login_page_html);
    if (login_page_len < max_result_len) {
        snprintf(result, max_result_len, "%s", login_page_html);
    } else {
        // Handle the case where the HTML content is too large for the buffer
        snprintf(result, max_result_len, "HTML content is too large");
    }
}

// Callback function when data is received from client
static int test_server_content(const char *request, const char *params, char *result, size_t max_result_len) {
    int len = 0;

    if (strncmp(request, LOGIN_PAGE_REQUEST, sizeof(LOGIN_PAGE_REQUEST) - 1) == 0) {
        // Generate result using the new function
        generate_login_page_html(result, max_result_len);
        len = strlen(result);
    } else if (strncmp(request, "/submit", sizeof("/submit") - 1) == 0) {
        // Simplified response for the /submit page
        const char *submit_page_html = 
        "<html>\n"
        "<head>\n"
        "    <title>Login Success!</title>\n"
        "    <style>\n"
        "        body {\n"
        "            font-family: 'Arial', sans-serif;\n"
        "            background-color: #f5f5f5;\n"
        "            text-align: center;\n"
        "            margin-top: 50px;\n"
        "        }\n"
        "\n"
        "        h2 {\n"
        "            color: #006633; /* Starbucks green */\n"
        "        }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <h2>Login Success!</h2>\n"
        "</body>\n"
        "</html>";
        len = strlen(submit_page_html);

        if (len < max_result_len) {
            snprintf(result, max_result_len, "%s", submit_page_html);
        } else {
            snprintf(result, max_result_len, "HTML content is too large");
        }
    }

    return len;
}

// Callback function when client connection is polled
err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T*)arg;
    if (!p) {
        DEBUG_printf("Connection closed\n");
        return tcp_close_client_connection(con_state, pcb, ERR_OK);
    }
    assert(con_state && con_state->pcb == pcb);

    if (p->tot_len > 0) {
        DEBUG_printf("Received data: %d bytes\n", p->tot_len);

        // Add debug message to print the received data
        for (struct pbuf *q = p; q != NULL; q = q->next) {
            DEBUG_printf("Received: %.*s\n", q->len, q->payload);
        }

        // Copy the request into the buffer
        pbuf_copy_partial(p, con_state->headers, p->tot_len > sizeof(con_state->headers) - 1 ? sizeof(con_state->headers) - 1 : p->tot_len, 0);

        // Handle GET request
        if (strncmp(HTTP_GET, con_state->headers, sizeof(HTTP_GET) - 1) == 0) {
            char *request = con_state->headers + sizeof(HTTP_GET); // + space
            char *params = strchr(request, '?');
            if (params) {
                if (*params) {
                    char *space = strchr(request, ' ');
                    *params++ = 0;
                    if (space) {
                        *space = 0;
                    }
                } else {
                    params = NULL;
                }
            }

            // Generate content
            con_state->result_len = test_server_content(request, params, con_state->result, sizeof(con_state->result));
            DEBUG_printf("Request: %s?%s\n", request, params);
            DEBUG_printf("Result: %d\n", con_state->result_len);

            // Check we had enough buffer space
            if (con_state->result_len > sizeof(con_state->result) - 1) {
                DEBUG_printf("Too much result data %d\n", con_state->result_len);
                return tcp_close_client_connection(con_state, pcb, ERR_CLSD);
            }

            // Generate web page
            if (con_state->result_len > 0) {
                con_state->header_len = snprintf(con_state->headers, sizeof(con_state->headers), HTTP_RESPONSE_HEADERS,
                    200, con_state->result_len);
                if (con_state->header_len > sizeof(con_state->headers) - 1) {
                    DEBUG_printf("Too much header data %d\n", con_state->header_len);
                    return tcp_close_client_connection(con_state, pcb, ERR_CLSD);
                }
            } else {
                // Send redirect
                con_state->header_len = snprintf(con_state->headers, sizeof(con_state->headers), HTTP_RESPONSE_REDIRECT,
                    ipaddr_ntoa(con_state->gw));
                DEBUG_printf("Sending redirect %s", con_state->headers);
            }

            // Send the headers to the client
            con_state->sent_len = 0;
            err_t err = tcp_write(pcb, con_state->headers, con_state->header_len, 0);
            if (err != ERR_OK) {
                DEBUG_printf("Failed to write header data %d\n", err);
                return tcp_close_client_connection(con_state, pcb, err);
            }

            // Send the body to the client
            if (con_state->result_len) {
                err = tcp_write(pcb, con_state->result, con_state->result_len, 0);
                if (err != ERR_OK) {
                    DEBUG_printf("Failed to write result data %d\n", err);
                    return tcp_close_client_connection(con_state, pcb, err);
                }
            }
        }
        tcp_recved(pcb, p->tot_len);
    }
    pbuf_free(p);
    return ERR_OK;
}

// Callback function for handling errors in client connection
static err_t tcp_server_poll(void *arg, struct tcp_pcb *pcb) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T*)arg;
    DEBUG_printf("tcp_server_poll_fn\n");
    return tcp_close_client_connection(con_state, pcb, ERR_OK); // Just disconnect clent?
}

static void tcp_server_err(void *arg, err_t err) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T*)arg;
    if (err != ERR_ABRT) {
        DEBUG_printf("tcp_client_err_fn %d\n", err);
        tcp_close_client_connection(con_state, con_state->pcb, err);
    }
}

// Callback function when new client connection is accepted
static err_t tcp_server_accept(void *arg, struct tcp_pcb *client_pcb, err_t err) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    if (err != ERR_OK || client_pcb == NULL) {
        DEBUG_printf("Failure in accept\n");
        return ERR_VAL;
    }
    DEBUG_printf("Client connected\n");

    // Create the state for the connection
    TCP_CONNECT_STATE_T *con_state = calloc(1, sizeof(TCP_CONNECT_STATE_T));
    if (!con_state) {
        DEBUG_printf("failed to allocate connect state\n");
        return ERR_MEM;
    }
    con_state->pcb = client_pcb; // for checking
    con_state->gw = &state->gw;

    // setup connection to client
    tcp_arg(client_pcb, con_state);
    tcp_sent(client_pcb, tcp_server_sent);
    tcp_recv(client_pcb, tcp_server_recv);
    tcp_poll(client_pcb, tcp_server_poll, POLL_TIME_S * 2);
    tcp_err(client_pcb, tcp_server_err);

    return ERR_OK;
}

// Function to open TCP server
static bool tcp_server_open(void *arg, const char *ap_name) {
    TCP_SERVER_T *state = (TCP_SERVER_T*)arg;
    DEBUG_printf("starting server on port %d\n", TCP_PORT);

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);
    if (!pcb) {
        DEBUG_printf("failed to create pcb\n");
        return false;
    }

    err_t err = tcp_bind(pcb, IP_ANY_TYPE, TCP_PORT);
    if (err) {
        DEBUG_printf("failed to bind to port %d\n",TCP_PORT);
        return false;
    }

    state->server_pcb = tcp_listen_with_backlog(pcb, 1);
    if (!state->server_pcb) {
        DEBUG_printf("failed to listen\n");
        if (pcb) {
            tcp_close(pcb);
        }
        return false;
    }

    tcp_arg(state->server_pcb, state);
    tcp_accept(state->server_pcb, tcp_server_accept);

    printf("Try connecting to '%s' (press 'd' to disable access point)\n", ap_name);
    return true;
}

// This "worker" function is called to safely perform work when instructed by key_pressed_func
void key_pressed_worker_func(async_context_t *context, async_when_pending_worker_t *worker) {
    assert(worker->user_data);
    printf("Disabling wifi\n");
    cyw43_arch_disable_ap_mode();
    ((TCP_SERVER_T*)(worker->user_data))->complete = true;
}

static async_when_pending_worker_t key_pressed_worker = {
        .do_work = key_pressed_worker_func
};

void key_pressed_func(void *param) {
    assert(param);
    int key = getchar_timeout_us(0); // get any pending key press but don't wait
    if (key == 'd' || key == 'D') {
        // We are probably in irq context so call wifi in a "worker"
        async_context_set_work_pending(((TCP_SERVER_T*)param)->context, &key_pressed_worker);
    }
}

int main() {
    stdio_init_all();

    TCP_SERVER_T *state = calloc(1, sizeof(TCP_SERVER_T));
    if (!state) {
        DEBUG_printf("failed to allocate state\n");
        return 1;
    }

    if (cyw43_arch_init()) {
        DEBUG_printf("failed to initialise\n");
        return 1;
    }

    // Get notified if the user presses a key
    state->context = cyw43_arch_async_context();
    key_pressed_worker.user_data = state;
    async_context_add_when_pending_worker(cyw43_arch_async_context(), &key_pressed_worker);
    stdio_set_chars_available_callback(key_pressed_func, state);

    const char *ap_name = "MyRepublic@Starbucks";
#if 1
    const char *password = NULL;
#else
    const char *password = "password";
#endif

    cyw43_arch_enable_ap_mode(ap_name, password, CYW43_AUTH_WPA2_AES_PSK);

    ip4_addr_t mask;
    IP4_ADDR(ip_2_ip4(&state->gw), 192, 168, 4, 1);
    IP4_ADDR(ip_2_ip4(&mask), 255, 255, 255, 0);

    // Start the dhcp server
    dhcp_server_t dhcp_server;
    dhcp_server_init(&dhcp_server, &state->gw, &mask);

    // Start the dns server
    dns_server_t dns_server;
    dns_server_init(&dns_server, &state->gw);

    if (!tcp_server_open(state, ap_name)) {
        DEBUG_printf("failed to open server\n");
        return 1;
    }

    state->complete = false;
    while(!state->complete) {
        // the following #ifdef is only here so this same example can be used in multiple modes;
        // you do not need it in your code
#if PICO_CYW43_ARCH_POLL
        // if you are using pico_cyw43_arch_poll, then you must poll periodically from your
        // main loop (not from a timer interrupt) to check for Wi-Fi driver or lwIP work that needs to be done.
        cyw43_arch_poll();
        // you can poll as often as you like, however if you have nothing else to do you can
        // choose to sleep until either a specified time, or cyw43_arch_poll() has work to do:
        cyw43_arch_wait_for_work_until(make_timeout_time_ms(1000));
#else
        // if you are not using pico_cyw43_arch_poll, then Wi-FI driver and lwIP work
        // is done via interrupt in the background. This sleep is just an example of some (blocking)
        // work you might be doing.
        sleep_ms(1000);
#endif
    }
    tcp_server_close(state);
    dns_server_deinit(&dns_server);
    dhcp_server_deinit(&dhcp_server);
    cyw43_arch_deinit();
    return 0;
}