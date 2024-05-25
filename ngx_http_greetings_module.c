// By: Nikit Swaraj for Nginx tutorial video
//
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

// Function to handle the directive
static ngx_int_t ngx_http_greetings_handler(ngx_http_request_t *r) {
    // Setting the content type to HTML
    r->headers_out.content_type_len = sizeof("text/html") - 1;
    r->headers_out.content_type.data = (u_char *)"text/html";
    r->headers_out.content_type.len = sizeof("text/html") - 1;

    // Creating the HTML content
    const char *response_str = "<html><body><h1 style=\"font-size:48px;\">Hello, Welcome to the world of Nginx!</h1></body></html>";

    // Creating the response buffer
    ngx_buf_t *b;
    ngx_chain_t out;

    b = ngx_pcalloc(r->pool, sizeof(ngx_buf_t));
    out.buf = b;
    out.next = NULL;

    b->pos = (u_char *)response_str;
    b->last = b->pos + strlen(response_str);
    b->memory = 1;
    b->last_buf = 1;

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = b->last - b->pos;

    // Sending the response
    ngx_http_send_header(r);
    return ngx_http_output_filter(r, &out);
}

// Directive definition
static char *ngx_http_greetings(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_http_core_loc_conf_t *clcf;

    // Find the location configuration for this request
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);

    // Set the handler for the directive
    clcf->handler = ngx_http_greetings_handler;

    return NGX_CONF_OK;
}

// Module directives
static ngx_command_t ngx_http_greetings_commands[] = {
    { ngx_string("greetings"),
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
      ngx_http_greetings,
      0,
      0,
      NULL },
    ngx_null_command
};

// Module context
static ngx_http_module_t ngx_http_greetings_module_ctx = {
    NULL,                          // preconfiguration
    NULL,                          // postconfiguration
    NULL,                          // create main configuration
    NULL,                          // init main configuration
    NULL,                          // create server configuration
    NULL,                          // merge server configuration
    NULL,                          // create location configuration
    NULL                           // merge location configuration
};

// Module definition
ngx_module_t ngx_http_greetings_module = {
    NGX_MODULE_V1,
    &ngx_http_greetings_module_ctx, // module context
    ngx_http_greetings_commands,    // module directives
    NGX_HTTP_MODULE,                // module type
    NULL,                           // init master
    NULL,                           // init module
    NULL,                           // init process
    NULL,                           // init thread
    NULL,                           // exit thread
    NULL,                           // exit process
    NULL,                           // exit master
    NGX_MODULE_V1_PADDING
};

