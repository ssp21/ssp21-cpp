#ifndef SSP21PROXY_CONFIGPROPERTIES_H
#define SSP21PROXY_CONFIGPROPERTIES_H

namespace props
{
    static const char* log_levels = "log_levels";
    static const char* proto_type = "proto_type";
    static const char* mode = "mode";
    static const char* crypto_only = "crypto_only";
    static const char* handshake_mode = "handshake_mode";
    static const char* local_public_key_path = "local_public_key_path";
    static const char* local_private_key_path = "local_private_key_path";
    static const char* remote_public_key_path = "remote_public_key_path";
    static const char* shared_secret_key_path = "shared_secret_key_path";
    static const char* authority_cert_path = "authority_cert_path";
    static const char* local_cert_path = "local_cert_path";
    static const char* local_address = "local_address";
    static const char* remote_address = "remote_address";
    static const char* serial_port = "serial_port";

    // TCP proxy
    static const char* max_sessions = "max_sessions";
    static const char* listen_port = "listen_port";
    static const char* listen_endpoint = "listen_endpoint";
    static const char* connect_port = "connect_port";
    static const char* connect_endpoint = "connect_endpoint";

    // UDP proxy
    static const char* raw_rx_ip = "raw.rx.ip";
    static const char* raw_rx_port = "raw.rx.port";
    static const char* raw_tx_ip = "raw.tx.ip";
    static const char* raw_tx_port = "raw.tx.port";
    static const char* secure_rx_ip = "secure.rx.ip";
    static const char* secure_rx_port = "secure.rx.port";
    static const char* secure_tx_ip = "secure.tx.ip";
    static const char* secure_tx_port = "secure.tx.port";
}

#endif
