#define _GNU_SOURCE
#include <microhttpd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "mathlib.h"

#define PORT 8080
#define RESPONSE_BUF 256

/* Helper to extract a named GET argument from connection into dest buffer.
 * Returns 1 if found, 0 otherwise.
 */
struct arg_capture {
    const char *name;
    char *dest;
    size_t maxlen;
};

static int
kv_iter(void *cls, enum MHD_ValueKind kind, const char *key, const char *value)
{
    (void)kind;
    struct arg_capture *cap = (struct arg_capture *)cls;
    if (0 == strcmp(key, cap->name)) {
        strncpy(cap->dest, value, cap->maxlen - 1);
        cap->dest[cap->maxlen - 1] = '\0';
        return MHD_NO; /* stop iteration */
    }
    return MHD_YES; /* continue */
}

static int get_query_arg(struct MHD_Connection *connection, const char *name, char *buf, size_t buflen) {
    struct arg_capture cap = { .name = name, .dest = buf, .maxlen = buflen };
    int rc = MHD_get_connection_values(connection, MHD_GET_ARGUMENT_KIND, &kv_iter, &cap);
    /* MHD_get_connection_values returns MHD_YES on success */
    (void)rc;
    return buf[0] ? 1 : 0;
}

/* Handler for incoming requests */
static int answer_to_connection(void *cls,
                                struct MHD_Connection *connection,
                                const char *url,
                                const char *method,
                                const char *version,
                                const char *upload_data,
                                size_t *upload_data_size, void **con_cls)
{
    (void)cls; (void)version; (void)upload_data; (void)upload_data_size; (void)con_cls;

    char reply[RESPONSE_BUF];
    memset(reply, 0, sizeof(reply));

    if (strcmp(url, "/health") == 0) {
        snprintf(reply, sizeof(reply), "{\"status\":\"ok\"}");
    }
    else if (strcmp(url, "/add") == 0 || strcmp(url, "/sub") == 0 ||
             strcmp(url, "/mul") == 0 || strcmp(url, "/div") == 0) {
        char a_str[64] = {0}, b_str[64] = {0};
        long a = 0, b = 0;
        if (!get_query_arg(connection, "a", a_str, sizeof(a_str)) ||
            !get_query_arg(connection, "b", b_str, sizeof(b_str))) {
            snprintf(reply, sizeof(reply), "{\"error\":\"missing a or b parameter\"}");
        } else {
            errno = 0;
            a = strtol(a_str, NULL, 10);
            if (errno != 0) { snprintf(reply, sizeof(reply), "{\"error\":\"invalid a\"}"); goto done; }
            errno = 0;
            b = strtol(b_str, NULL, 10);
            if (errno != 0) { snprintf(reply, sizeof(reply), "{\"error\":\"invalid b\"}"); goto done; }

            if (strcmp(url, "/add") == 0) {
                long r = math_add(a, b);
                snprintf(reply, sizeof(reply), "{\"result\":%ld}", r);
            } else if (strcmp(url, "/sub") == 0) {
                long r = math_sub(a, b);
                snprintf(reply, sizeof(reply), "{\"result\":%ld}", r);
            } else if (strcmp(url, "/mul") == 0) {
                long r = math_mul(a, b);
                snprintf(reply, sizeof(reply), "{\"result\":%ld}", r);
            } else { /* /div */
                int err = 0;
                long r = math_div(a, b, &err);
                if (err) snprintf(reply, sizeof(reply), "{\"error\":\"division_by_zero\"}");
                else snprintf(reply, sizeof(reply), "{\"result\":%ld}", r);
            }
        }
    } else {
        snprintf(reply, sizeof(reply), "{\"error\":\"unknown endpoint\"}");
    }

done:
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(reply),
                                                                    (void *)reply,
                                                                    MHD_RESPMEM_MUST_COPY);
    if (!response) return MHD_NO;
    MHD_add_response_header(response, "Content-Type", "application/json; charset=utf-8");
    int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    struct MHD_Daemon *daemon;

    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                              &answer_to_connection, NULL, MHD_OPTION_END);
    if (NULL == daemon) {
        fprintf(stderr, "Failed to start HTTP server on port %d\n", PORT);
        return 1;
    }

    printf("Server running on http://0.0.0.0:%d/\n", PORT);
    printf("Endpoints:\n  /health\n  /add?a=1&b=2\n  /sub?a=5&b=3\n  /mul?a=2&b=10\n  /div?a=10&b=2\n");
    printf("Press Enter to stop the server.\n");
    getchar();

    MHD_stop_daemon(daemon);
    return 0;
}
