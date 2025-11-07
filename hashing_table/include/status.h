#ifndef STATUS_H
#define STATUS_H

typedef enum {
    DSA_OK,
    DSA_ERROR,
    DSA_BAD_PARAM,
    DSA_EXISTS,
    DSA_NOT_FOUND
} dsa_status_t;

#endif // STATUS_H