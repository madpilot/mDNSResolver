#ifndef MDNS_CONSTANTS_h
#define MDNS_CONSTANTS_h

#ifndef MDNS_RESOLVER_MAX_CACHE
#define MDNS_RESOLVER_MAX_CACHE           4
#endif

#define MDNS_MAX_NAME_LEN                 255

#define E_MDNS_OK                         0x20
#define E_MDNS_TOO_BIG                    0x21
#define E_MDNS_POINTER_OVERFLOW           0x22
#define E_MDNS_PACKET_ERROR               0x23
#define E_MDNS_PARSING_ERROR              0x24
#define E_MDNS_TRUNCATED                  0x25
#define E_MDNS_INVALID_LABEL_LENGTH       0x26
#define E_MDNS_OUT_OF_MEMORY              0x27

#define MDNS_A_RECORD                     0x01
#define MDNS_CNAME_RECORD                 0x05

#define MDNS_RESULT                       unsigned int
#endif
