#ifndef VC_RANGE_PAGE_H
#define VC_RANGE_PAGE_H

void vc_range_page_init(void);
void vc_range_page_update(void);

void vc_range_page_handle_right(void);
void vc_range_page_handle_select(void);

void set_voltage_min_limit(float value);
void set_voltage_max_limit(float value);

void set_current_min_limit(float value);
void set_current_max_limit(float value);

float get_voltage_min_limit(void);
float get_voltage_max_limit(void);

float get_current_min_limit(void);
float get_current_max_limit(void);

#endif