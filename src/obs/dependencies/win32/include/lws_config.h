/* lws_config.h  Generated from lws_config.h.in  */

#ifndef NDEBUG
	#ifndef _DEBUG
		#define _DEBUG
	#endif
#endif

#define LWS_INSTALL_DATADIR "C:/Program Files (x86)/libwebsockets/share"
#define LWS_LIBRARY_VERSION_MAJOR 3
#define LWS_LIBRARY_VERSION_MINOR 1
#define LWS_LIBRARY_VERSION_PATCH 99
/* LWS_LIBRARY_VERSION_NUMBER looks like 1005001 for e.g. version 1.5.1 */
#define LWS_LIBRARY_VERSION_NUMBER (LWS_LIBRARY_VERSION_MAJOR * 1000000) + \
					(LWS_LIBRARY_VERSION_MINOR * 1000) + \
					LWS_LIBRARY_VERSION_PATCH
#define LWS_MAX_SMP 1

/* #undef LWS_AVOID_SIGPIPE_IGN */
#define LWS_BUILD_HASH "v3.1.0-114-g737e3613"
#define LWS_BUILTIN_GETIFADDRS
/* #undef LWS_FALLBACK_GETHOSTBYNAME */
#define LWS_HAS_INTPTR_T
/* #undef LWS_HAS_GETOPT_LONG */
#define LWS_HAVE__ATOI64
#define LWS_HAVE_ATOLL
/* #undef LWS_HAVE_BN_bn2binpad */
/* #undef LWS_HAVE_EC_POINT_get_affine_coordinates */
/* #undef LWS_HAVE_ECDSA_SIG_set0 */
/* #undef LWS_HAVE_EVP_MD_CTX_free */
/* #undef LWS_HAVE_EVP_aes_128_wrap */
/* #undef LWS_HAVE_LIBCAP */
#define LWS_HAVE_MALLOC_H
/* #undef LWS_HAVE_MALLOC_TRIM */
/* #undef LWS_HAVE_MALLOC_USABLE_SIZE */
#define LWS_HAVE_mbedtls_net_init
#define LWS_HAVE_mbedtls_ssl_conf_alpn_protocols
#define LWS_HAVE_mbedtls_ssl_get_alpn_protocol
#define LWS_HAVE_mbedtls_ssl_conf_sni
#define LWS_HAVE_mbedtls_ssl_set_hs_ca_chain
#define LWS_HAVE_mbedtls_ssl_set_hs_own_cert
#define LWS_HAVE_mbedtls_ssl_set_hs_authmode
/* #undef LWS_HAVE_NEW_UV_VERSION_H */
/* #undef LWS_HAVE_OPENSSL_ECDH_H */
/* #undef LWS_HAVE_PIPE2 */
/* #undef LWS_HAVE_PTHREAD_H */
/* #undef LWS_HAVE_RSA_SET0_KEY */
/* #undef LWS_HAVE_SSL_CTX_get0_certificate */
/* #undef LWS_HAVE_SSL_CTX_set1_param */
/* #undef LWS_HAVE_SSL_CTX_set_ciphersuites */
/* #undef LWS_HAVE_SSL_EXTRA_CHAIN_CERTS */
/* #undef LWS_HAVE_SSL_get0_alpn_selected */
/* #undef LWS_HAVE_SSL_set_alpn_protos */
/* #undef LWS_HAVE_SSL_SET_INFO_CALLBACK */
#define LWS_HAVE__STAT32I64
#define LWS_HAVE_STDINT_H
/* #undef LWS_HAVE_SYS_CAPABILITY_H */
#define LWS_HAVE_TLS_CLIENT_METHOD
/* #undef LWS_HAVE_TLSV1_2_CLIENT_METHOD */
/* #undef LWS_HAVE_UV_VERSION_H */
/* #undef LWS_HAVE_X509_get_key_usage */
#define LWS_HAVE_X509_VERIFY_PARAM_set1_host
/* #undef LWS_LATENCY */
#define LWS_LIBRARY_VERSION "3.1.99"
/* #undef LWS_MINGW_SUPPORT */
/* #undef LWS_NO_CLIENT */
#define LWS_NO_DAEMONIZE
/* #undef LWS_NO_SERVER */
#define LWS_OPENSSL_CLIENT_CERTS "../share"
#define LWS_OPENSSL_SUPPORT
/* #undef LWS_PLAT_OPTEE */
/* #undef LWS_ROLE_CGI */
/* #undef LWS_ROLE_DBUS */
#define LWS_ROLE_H1
#define LWS_ROLE_H2
#define LWS_ROLE_RAW
/* #undef LWS_ROLE_RAW_PROXY */
#define LWS_ROLE_WS
/* #undef LWS_SHA1_USE_OPENSSL_NAME */
#define LWS_SSL_CLIENT_USE_OS_CA_CERTS
/* #undef LWS_SSL_SERVER_WITH_ECDH_CERT */
/* #undef LWS_WITH_ACCESS_LOG */
/* #undef LWS_WITH_ACME */
/* #undef LWS_WITH_BORINGSSL */
/* #undef LWS_WITH_CGI */
#define LWS_WITH_CUSTOM_HEADERS
/* #undef LWS_WITH_DIR */
/* #undef LWS_WITH_ESP32 */
/* #undef LWS_WITH_FTS */
/* #undef LWS_WITH_GENCRYPTO */
#define LWS_WITH_HTTP2
/* #undef LWS_WITH_HTTP_BROTLI */
/* #undef LWS_WITH_HTTP_PROXY */
/* #undef LWS_WITH_HTTP_STREAM_COMPRESSION */
/* #undef LWS_WITH_IPV6 */
/* #undef LWS_WITH_JOSE */
#define LWS_WITH_LEJP
/* #undef LWS_WITH_LIBEV */
/* #undef LWS_WITH_LIBEVENT */
/* #undef LWS_WITH_LIBUV */
#define LWS_WITH_LWSAC
#define LWS_WITH_MBEDTLS
#define LWS_WITH_NETWORK
/* #undef LWS_WITH_NO_LOGS */
/* #undef LWS_WITHOUT_CLIENT */
#define LWS_WITHOUT_EXTENSIONS
/* #undef LWS_WITHOUT_SERVER */
/* #undef LWS_WITH_PEER_LIMITS */
/* #undef LWS_WITH_PLUGINS */
/* #undef LWS_WITH_POLARSSL */
#define LWS_WITH_POLL
/* #undef LWS_WITH_RANGES */
/* #undef LWS_WITH_SELFTESTS */
/* #undef LWS_WITH_SERVER_STATUS */
/* #undef LWS_WITH_SMTP */
/* #undef LWS_WITH_SOCKS5 */
/* #undef LWS_WITH_STATEFUL_URLDECODE */
/* #undef LWS_WITH_STATS */
/* #undef LWS_WITH_THREADPOOL */
#define LWS_WITH_TLS
/* #undef LWS_WITH_UNIX_SOCK */
/* #undef LWS_WITH_ZIP_FOPS */
/* #undef USE_OLD_CYASSL */
/* #undef USE_WOLFSSL */


