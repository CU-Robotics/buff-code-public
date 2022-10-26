#ifndef REF_SYS_H
#define REF_SYS_H


struct ref_data{

    char curr_stage;
    //char comp_type;
    uint16_t rem_time;
    //char comp_result;

    uint8_t curr_robot_id;
    uint8_t curr_robot_level;
    uint16_t curr_robot_hp;
    uint16_t curr_robot_17_cool_val;
    uint16_t curr_robot_17_heat_lim;
    uint16_t curr_robot_17_speed_lim;
    uint16_t second_wpn_cool_val;
    uint16_t second_wpn_heat_lim;
    uint16_t second_wpn_speed_lim;

    //int red_hero_hp;

    uint16_t red_sentry_hp;

    //int red_infantry_hp;
    //int blue_hero_hp;

    uint16_t blue_sentry_hp;

    // int blue_infantry_hp;
    // int red_hero_max_hp;
    // int red_sentry_max_hp;
    // int red_infantry_max_hp;
    // int blue_hero_max_hp;
    // int blue_sentry_max_hp;
    // int blue_infantry_max_hp;
    // int red_one_rem_proj;
    // int red_two_rem_proj;
    // int blue_one_rem_proj;
    // int blue_two_rem_proj;

    char ref_warning;
    uint8_t foul_robot_id;

    // int red_hero_robot_level;
    // int red_infantry_robot_level;
    // int red_sentry_robot_level;
    // int blue_hero_robot_level;
    // int blue_infantry_robot_level;
    // int blue_sentry_robot_level;

    // int robot_1_cool_val;       //17mm
    // int robot_1_barr_heat_lim;       //17mm
    // int robot_1_speed_lim;       //17mm

    // int robot_2_cool_val;       //17mm
    // int robot_2_barr_heat_lim;       //17mm
    // int robot_2_speed_lim;       //17mm

    // int robot_42_cool_val;
    // int robot_42_heat_lim;
    // int robot_42_speed_lim;  

    uint16_t robot_power_lim;

    uint16_t chasis_volt;
    uint16_t chasis_current;

    uint32_t chasis_power;

    uint16_t curr_robot_barr_heat;
    uint16_t curr_robot_second_barr_heat;

    //int robot_buff;

    uint8_t launch_id;

    uint8_t launch_freq;
    uint32_t launch_speed;

    //int rem_17_proj;
    //int rem_42_proj;


 };

class ref_sys{

    public:

    void init();

    bool read_serial();

    bool hid_buff_write(byte  arr[]);  //maybe take a buffer pointer as an input (in progress)

    private:

    ref_data run_data;


};

#endif