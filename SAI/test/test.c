#include <stdio.h>
#include "sai.h"
#include "../stub/inc/stub_sai.h"

const char* test_profile_get_value(_In_ sai_switch_profile_id_t profile_id, _In_ const char* variable) { return 0; }
int test_profile_get_next_value(_In_ sai_switch_profile_id_t profile_id, _Out_ const char** variable, _Out_ const char** value) { return -1; }
const service_method_table_t test_services = { test_profile_get_value, test_profile_get_next_value };

int main()
{
    sai_status_t      status;
    sai_switch_api_t *switch_api = NULL;
    sai_lag_api_t    *lag_api = NULL;
    
    sai_object_id_t   lag1 = SAI_NULL_OBJECT_ID, lag2 = SAI_NULL_OBJECT_ID;
    sai_object_id_t   mem1, mem2, mem3, mem4;

    sai_attribute_t   attr_list[2] = {0};
    sai_attribute_t   sw_attr[1] = {0};
    sai_object_id_t   port_list[PORT_NUMBER] = {0};

    status = sai_api_initialize(0, &test_services);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to initialize SAI API, status=%d\n", status);
        return 1;
    }

    status = sai_api_query(SAI_API_SWITCH, (void**)&switch_api);
    if (status != SAI_STATUS_SUCCESS || switch_api == NULL) {
        printf("Failed to query SWITCH API, status=%d\n", status);
        return 1;
    }

    status = sai_api_query(SAI_API_LAG, (void**)&lag_api);
    if (status != SAI_STATUS_SUCCESS || lag_api == NULL) {
        printf("Failed to query LAG API, status=%d\n", status);
        return 1;
    }

    sai_switch_notification_t notifications = {0};
    status = switch_api->initialize_switch(0, "HW_ID", 0, &notifications);
    
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to initialize switch, status=%d\n", status);
        return 1;
    }

    sw_attr[0].id = SAI_SWITCH_ATTR_PORT_LIST;
    sw_attr[0].value.objlist.count = PORT_NUMBER;
    sw_attr[0].value.objlist.list  = port_list;

    status = switch_api->get_switch_attribute(1, sw_attr);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to get switch port list, status=%d\n", status);
        return 1;
    }
    printf("\n");

    status = lag_api->create_lag(&lag1, 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG#1, status=%d\n", status);
        return 1;
    }
    printf("Created LAG#1: 0x%lX\n", lag1);

    status = lag_api->create_lag(&lag2, 0, NULL);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG#2, status=%d\n", status);
        return 1;
    }
    printf("Created LAG#2: 0x%lX\n", lag2);

    attr_list[0].id = SAI_LAG_MEMBER_ATTR_PORT_ID;
    attr_list[0].value.oid = port_list[0];
    attr_list[1].id = SAI_LAG_MEMBER_ATTR_LAG_ID;
    attr_list[1].value.oid = lag1;
    status = lag_api->create_lag_member(&mem1, 2, attr_list);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#1, status=%d\n", status);
        return 1;
    }
    printf("Created LAG_MEMBER#1: 0x%lX (LAG#1, PORT[0])\n", mem1);

    attr_list[0].value.oid = port_list[1];
    attr_list[1].value.oid = lag1;
    status = lag_api->create_lag_member(&mem2, 2, attr_list);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#2, status=%d\n", status);
        return 1;
    }
    printf("Created LAG_MEMBER#2: 0x%lX (LAG#1, PORT[1])\n", mem2);

    attr_list[0].value.oid = port_list[2];
    attr_list[1].value.oid = lag2;
    status = lag_api->create_lag_member(&mem3, 2, attr_list);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#3, status=%d\n", status);
        return 1;
    }
    printf("Created LAG_MEMBER#3: 0x%lX (LAG#2, PORT[2])\n", mem3);

    attr_list[0].value.oid = port_list[3];
    attr_list[1].value.oid = lag2;
    status = lag_api->create_lag_member(&mem4, 2, attr_list);
    if (status != SAI_STATUS_SUCCESS) {
        printf("Failed to create LAG_MEMBER#4, status=%d\n", status);
        return 1;
    }
    printf("Created LAG_MEMBER#4: 0x%lX (LAG#2, PORT[3])\n", mem4);

    printf("\n");

    {
        sai_object_id_t lag_ports[PORT_NUMBER];
        sai_attribute_t lag_attr = {0};

        lag_attr.id = SAI_LAG_ATTR_PORT_LIST;
        lag_attr.value.objlist.count = PORT_NUMBER;
        lag_attr.value.objlist.list  = lag_ports;

        status = lag_api->get_lag_attribute(lag1, 1, &lag_attr);
        if (status == SAI_STATUS_BUFFER_OVERFLOW) {
            printf("LAG#1 PORT_LIST buffer too small, need %u\n", lag_attr.value.objlist.count);
        } else if (status != SAI_STATUS_SUCCESS) {
            printf("Failed to get LAG#1 PORT_LIST, status=%d\n", status);
        } else {
            printf("LAG#1 PORT_LIST (%u ports): ", lag_attr.value.objlist.count);
            for (uint32_t i = 0; i < lag_attr.value.objlist.count; i++) {
                printf("0x%lX ", lag_ports[i]);
            }
            printf("\n");
        }
    }

    {
        sai_object_id_t lag_ports[PORT_NUMBER];
        sai_attribute_t lag_attr = {0};

        lag_attr.id = SAI_LAG_ATTR_PORT_LIST;
        lag_attr.value.objlist.count = PORT_NUMBER;
        lag_attr.value.objlist.list  = lag_ports;

        status = lag_api->get_lag_attribute(lag2, 1, &lag_attr);
        if (status == SAI_STATUS_BUFFER_OVERFLOW) {
            printf("LAG#2 PORT_LIST buffer too small, need %u\n", lag_attr.value.objlist.count);
        } else if (status != SAI_STATUS_SUCCESS) {
            printf("Failed to get LAG#2 PORT_LIST, status=%d\n", status);
        } else {
            printf("LAG#2 PORT_LIST (%u ports): ", lag_attr.value.objlist.count);
            for (uint32_t i = 0; i < lag_attr.value.objlist.count; i++) {
                printf("0x%lX ", lag_ports[i]);
            }
            printf("\n");
        }
    }

    {
        sai_attribute_t m_attr = {0};

        m_attr.id = SAI_LAG_MEMBER_ATTR_LAG_ID;
        status = lag_api->get_lag_member_attribute(mem1, 1, &m_attr);
        if (status == SAI_STATUS_SUCCESS) {
            printf("LAG_MEMBER#1 LAG_ID: 0x%lX\n", m_attr.value.oid);
        } else {
            printf("Failed to get LAG_MEMBER#1 LAG_ID, status=%d\n", status);
        }

        m_attr.id = SAI_LAG_MEMBER_ATTR_PORT_ID;
        status = lag_api->get_lag_member_attribute(mem3, 1, &m_attr);
        if (status == SAI_STATUS_SUCCESS) {
            printf("LAG_MEMBER#3 PORT_ID: 0x%lX\n", m_attr.value.oid);
        } else {
            printf("Failed to get LAG_MEMBER#3 PORT_ID, status=%d\n", status);
        }
    }

    printf("\n");

    lag_api->remove_lag_member(mem2);

    {
        sai_object_id_t lag_ports[PORT_NUMBER];
        sai_attribute_t lag_attr = {0};

        lag_attr.id = SAI_LAG_ATTR_PORT_LIST;
        lag_attr.value.objlist.count = PORT_NUMBER;
        lag_attr.value.objlist.list  = lag_ports;

        status = lag_api->get_lag_attribute(lag1, 1, &lag_attr);
        printf("LAG#1 PORT_LIST after removing mem2: ");
        if (status == SAI_STATUS_SUCCESS) {
            printf("(%u ports): ", lag_attr.value.objlist.count);
            for (uint32_t i = 0; i < lag_attr.value.objlist.count; i++) {
                printf("0x%lX ", lag_ports[i]);
            }
        } else {
            printf("failed, status=%d", status);
        }
        printf("\n");
    }

    lag_api->remove_lag_member(mem3);

    {
        sai_object_id_t lag_ports[PORT_NUMBER];
        sai_attribute_t lag_attr = {0};

        lag_attr.id = SAI_LAG_ATTR_PORT_LIST;
        lag_attr.value.objlist.count = PORT_NUMBER;
        lag_attr.value.objlist.list  = lag_ports;

        status = lag_api->get_lag_attribute(lag2, 1, &lag_attr);
        printf("LAG#2 PORT_LIST after removing mem3: ");
        if (status == SAI_STATUS_SUCCESS) {
            printf("(%u ports): ", lag_attr.value.objlist.count);
            for (uint32_t i = 0; i < lag_attr.value.objlist.count; i++) {
                printf("0x%lX ", lag_ports[i]);
            }
        } else {
            printf("failed, status=%d", status);
        }
        printf("\n");
    }

    printf("\n");

    lag_api->remove_lag_member(mem1);
    lag_api->remove_lag_member(mem4);
    lag_api->remove_lag(lag2);
    lag_api->remove_lag(lag1);

    switch_api->shutdown_switch(0);
    sai_api_uninitialize();
    
    return 0;
}