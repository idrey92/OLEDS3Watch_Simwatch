#include "../esp_brookesia_app_settings.hpp"

#include "settings.h"
#include "rtc_lib.h"

namespace esp_brookesia::apps
{

    void Settings::showDateTimeScreen()
    {
            clearScreen();

            // Back
            lv_obj_t *back = lv_label_create(screen);

            lv_label_set_text(back, LV_SYMBOL_LEFT " Back");

            lv_obj_set_style_text_color(
                back,
                lv_palette_main(LV_PALETTE_BLUE),
                0);

            lv_obj_set_style_text_font(
                back,
                &lv_font_montserrat_20,
                0);

            lv_obj_align(back,
                        LV_ALIGN_TOP_LEFT,
                        10,
                        8);

            lv_obj_add_flag(back, LV_OBJ_FLAG_CLICKABLE);

            lv_obj_add_event_cb(back,
                                backEventCb,
                                LV_EVENT_CLICKED,
                                this);

            // Заголовок
            lv_obj_t *title = lv_label_create(screen);
            lv_label_set_text(title, "Date & Time");
            lv_obj_set_style_text_font(
                title,
                &esp_brookesia_font_maison_neue_book_32,
                0);

            lv_obj_align(title,
                LV_ALIGN_TOP_MID,
                0,
                36);

            // RTC
            struct tm now{};
            rtc_get_time(&now);

            char timeBuf[16];
            char dateBuf[32];

            snprintf(timeBuf,
                    sizeof(timeBuf),
                    "%02d:%02d",
                    now.tm_hour,
                    now.tm_min);
        
            createValueMenuItem(
            screen,
            "Time",
            timeBuf,
            timeEventCb);

            snprintf(dateBuf,
                    sizeof(dateBuf),
                    "%02d/%02d/%04d",
                    now.tm_mday,
                    now.tm_mon + 1,
                    now.tm_year + 1900);

            createValueMenuItem(
                screen,
                "Date",
                dateBuf,
                dateEventCb);

            createValueMenuItem(
                screen,
                "24-Hour Time",
                "Off",
                nullptr);

            createValueMenuItem(
                screen,
                "Automatic",
                "Off",
                nullptr);
    }

    void Settings::showTimeScreen()
    {
            clearScreen();

            lv_obj_t *back = lv_label_create(screen);
            lv_label_set_text(back, LV_SYMBOL_LEFT " Back");
            lv_obj_add_flag(back, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_add_event_cb(back, backEventCb, LV_EVENT_CLICKED, this);

            lv_obj_t *title = lv_label_create(screen);
            lv_label_set_text(title, "Time");
            lv_obj_set_style_text_font(
                title,
                &esp_brookesia_font_maison_neue_book_32,
                0);
            
            lv_obj_t *rollerRow = lv_obj_create(screen);

            lv_obj_remove_style_all(rollerRow);

            lv_obj_set_width(rollerRow, lv_pct(100));
            lv_obj_set_height(rollerRow, 270);

            lv_obj_set_layout(rollerRow, LV_LAYOUT_FLEX);
            lv_obj_set_flex_flow(rollerRow, LV_FLEX_FLOW_ROW);

            lv_obj_set_flex_align(
                rollerRow,
                LV_FLEX_ALIGN_SPACE_EVENLY,
                LV_FLEX_ALIGN_CENTER,
                LV_FLEX_ALIGN_CENTER);

            struct tm now{};
            rtc_get_time(&now);

            char options[256] = "";

            //
            // ----------- ЧАСЫ -----------
            //

            lv_obj_t *hourBox = lv_obj_create(rollerRow);
            lv_obj_remove_style_all(hourBox);

            lv_obj_set_size(hourBox, 150, 260);

            lv_obj_set_layout(hourBox, LV_LAYOUT_FLEX);
            lv_obj_set_flex_flow(hourBox, LV_FLEX_FLOW_COLUMN);

            lv_obj_set_flex_align(
                hourBox,
                LV_FLEX_ALIGN_START,
                LV_FLEX_ALIGN_CENTER,
                LV_FLEX_ALIGN_CENTER);

            lv_obj_t *hourLabel = lv_label_create(hourBox);
            lv_label_set_text(hourLabel, "Hour");

            for (int i = 0; i < 24; i++)
            {
                char tmp[8];
                sprintf(tmp, "%02d\n", i);
                strcat(options, tmp);
            }

            hourRoller = lv_roller_create(hourBox);

            lv_obj_set_size(hourRoller, 120, 220);

            lv_roller_set_visible_row_count(hourRoller, 5);

            lv_roller_set_options(
                hourRoller,
                options,
                LV_ROLLER_MODE_NORMAL);

            lv_roller_set_selected(
                hourRoller,
                now.tm_hour,
                LV_ANIM_OFF);
            lv_obj_add_event_cb(
                hourRoller,
                rollerEventCb,
                LV_EVENT_ALL,
                this);
                //
            // ----------- МИНУТЫ -----------
            //

            options[0] = '\0';

            lv_obj_t *minuteBox = lv_obj_create(rollerRow);
            lv_obj_remove_style_all(minuteBox);

            lv_obj_set_size(minuteBox, 150, 260);

            lv_obj_set_layout(minuteBox, LV_LAYOUT_FLEX);
            lv_obj_set_flex_flow(minuteBox, LV_FLEX_FLOW_COLUMN);

            lv_obj_set_flex_align(
                minuteBox,
                LV_FLEX_ALIGN_START,
                LV_FLEX_ALIGN_CENTER,
                LV_FLEX_ALIGN_CENTER);

            lv_obj_t *minuteLabel = lv_label_create(minuteBox);
            lv_label_set_text(minuteLabel, "Minute");

            for (int i = 0; i < 60; i++)
            {
                char tmp[8];
                sprintf(tmp, "%02d\n", i);
                strcat(options, tmp);
            }

            minuteRoller = lv_roller_create(minuteBox);

            lv_obj_set_size(minuteRoller, 120, 220);

            lv_roller_set_visible_row_count(minuteRoller, 5);

            lv_roller_set_options(
                minuteRoller,
                options,
                LV_ROLLER_MODE_NORMAL);

            lv_roller_set_selected(
                minuteRoller,
                now.tm_min,
                LV_ANIM_OFF);

            lv_obj_add_event_cb(
                minuteRoller,
                rollerEventCb,
                LV_EVENT_ALL,
                this);

            //save button
            lv_obj_t *saveBtn = lv_btn_create(screen);

            lv_obj_set_size(saveBtn, 180, 50);

            lv_obj_add_event_cb(
                saveBtn,
                saveTimeEventCb,
                LV_EVENT_CLICKED,
                this);

            lv_obj_t *label = lv_label_create(saveBtn);
            lv_label_set_text(label, "Save");
            lv_obj_center(label);
    }
    
    void Settings::showDateScreen()
    {
        clearScreen();

        //==========================================================
        // Header
        //==========================================================

        lv_obj_t *back = lv_label_create(screen);
        lv_label_set_text(back, LV_SYMBOL_LEFT " Back");
        lv_obj_add_flag(back, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(back, backEventCb, LV_EVENT_CLICKED, this);

        lv_obj_t *title = lv_label_create(screen);
        lv_label_set_text(title, "Date");
        lv_obj_set_style_text_font(
            title,
            &esp_brookesia_font_maison_neue_book_32,
            0);

        //==========================================================
        // Roller row
        //==========================================================

        lv_obj_t *rollerRow = lv_obj_create(screen);

        lv_obj_remove_style_all(rollerRow);

        lv_obj_set_width(rollerRow, lv_pct(100));
        lv_obj_set_height(rollerRow, 280);

        lv_obj_set_layout(rollerRow, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(rollerRow, LV_FLEX_FLOW_ROW);

        lv_obj_set_flex_align(
            rollerRow,
            LV_FLEX_ALIGN_SPACE_EVENLY,
            LV_FLEX_ALIGN_CENTER,
            LV_FLEX_ALIGN_CENTER);

        struct tm now{};
        rtc_get_time(&now);

        char options[512];

        //==========================================================
        // DAY
        //==========================================================

        lv_obj_t *dayBox = lv_obj_create(rollerRow);
        lv_obj_remove_style_all(dayBox);

        lv_obj_set_size(dayBox, 100, 260);

        lv_obj_set_layout(dayBox, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(dayBox, LV_FLEX_FLOW_COLUMN);

        lv_obj_set_flex_align(
            dayBox,
            LV_FLEX_ALIGN_START,
            LV_FLEX_ALIGN_CENTER,
            LV_FLEX_ALIGN_CENTER);

        lv_obj_t *dayLabel = lv_label_create(dayBox);
        lv_label_set_text(dayLabel, "Day");

        options[0] = '\0';

        for (int i = 1; i <= 31; i++)
        {
            char tmp[8];
            sprintf(tmp, "%02d\n", i);
            strcat(options, tmp);
        }

        dayRoller = lv_roller_create(dayBox);

        lv_obj_set_size(dayRoller, 90, 220);

        lv_roller_set_visible_row_count(dayRoller, 5);

        lv_roller_set_options(
            dayRoller,
            options,
            LV_ROLLER_MODE_NORMAL);

        lv_roller_set_selected(
            dayRoller,
            now.tm_mday - 1,
            LV_ANIM_OFF);

        lv_obj_add_event_cb(
            dayRoller,
            rollerEventCb,
            LV_EVENT_ALL,
            this);

        //==========================================================
        // MONTH
        //==========================================================

        lv_obj_t *monthBox = lv_obj_create(rollerRow);
        lv_obj_remove_style_all(monthBox);

        lv_obj_set_size(monthBox, 140, 260);

        lv_obj_set_layout(monthBox, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(monthBox, LV_FLEX_FLOW_COLUMN);

        lv_obj_set_flex_align(
            monthBox,
            LV_FLEX_ALIGN_START,
            LV_FLEX_ALIGN_CENTER,
            LV_FLEX_ALIGN_CENTER);

        lv_obj_t *monthLabel = lv_label_create(monthBox);
        lv_label_set_text(monthLabel, "Month");

        static const char *months =
            "January\n"
            "February\n"
            "March\n"
            "April\n"
            "May\n"
            "June\n"
            "July\n"
            "August\n"
            "September\n"
            "October\n"
            "November\n"
            "December";

        monthRoller = lv_roller_create(monthBox);

        lv_obj_set_size(monthRoller, 130, 220);

        lv_roller_set_visible_row_count(monthRoller, 5);

        lv_roller_set_options(
            monthRoller,
            months,
            LV_ROLLER_MODE_NORMAL);

        lv_roller_set_selected(
            monthRoller,
            now.tm_mon,
            LV_ANIM_OFF);

        lv_obj_add_event_cb(
            monthRoller,
            rollerEventCb,
            LV_EVENT_ALL,
            this);

        //==========================================================
        // YEAR
        //==========================================================

        lv_obj_t *yearBox = lv_obj_create(rollerRow);
        lv_obj_remove_style_all(yearBox);

        lv_obj_set_size(yearBox, 100, 260);

        lv_obj_set_layout(yearBox, LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(yearBox, LV_FLEX_FLOW_COLUMN);

        lv_obj_set_flex_align(
            yearBox,
            LV_FLEX_ALIGN_START,
            LV_FLEX_ALIGN_CENTER,
            LV_FLEX_ALIGN_CENTER);

        lv_obj_t *yearLabel = lv_label_create(yearBox);
        lv_label_set_text(yearLabel, "Year");

        options[0] = '\0';

        for (int y = 2024; y <= 2045; y++)
        {
            char tmp[16];
            sprintf(tmp, "%d\n", y);
            strcat(options, tmp);
        }

        yearRoller = lv_roller_create(yearBox);

        lv_obj_set_size(yearRoller, 90, 220);

        lv_roller_set_visible_row_count(yearRoller, 5);

        lv_roller_set_options(
            yearRoller,
            options,
            LV_ROLLER_MODE_NORMAL);

        lv_roller_set_selected(
            yearRoller,
            (now.tm_year + 1900) - 2024,
            LV_ANIM_OFF);

        lv_obj_add_event_cb(
            yearRoller,
            rollerEventCb,
            LV_EVENT_ALL,
            this);

        //==========================================================
        // Save
        //==========================================================

        lv_obj_t *saveBtn = lv_btn_create(screen);

        lv_obj_set_size(saveBtn, 180, 50);

        lv_obj_add_event_cb(
            saveBtn,
            saveDateEventCb,
            LV_EVENT_CLICKED,
            this);

        lv_obj_t *label = lv_label_create(saveBtn);
        lv_label_set_text(label, "Save");
        lv_obj_center(label);
    }    
    void Settings::timeEventCb(lv_event_t *event)
    {
        auto *self = static_cast<Settings *>(lv_event_get_user_data(event));
        self->showTimeScreen();
    }

    void Settings::dateEventCb(lv_event_t *event)
    {
        auto *self = static_cast<Settings *>(lv_event_get_user_data(event));

        self->showDateScreen();
    }

    void Settings::saveTimeEventCb(lv_event_t *event)
    {
        auto *self = static_cast<Settings *>(lv_event_get_user_data(event));

        uint16_t hour =
            lv_roller_get_selected(self->hourRoller);

        uint16_t minute =
            lv_roller_get_selected(self->minuteRoller);

        struct tm now{};
        rtc_get_time(&now);

        now.tm_hour = hour;
        now.tm_min = minute;
        now.tm_sec = 0;

        if (rtc_set_time(&now) == ESP_OK)
        {
            printf("Time saved %02d:%02d\n",
                now.tm_hour,
                now.tm_min);

            self->showDateTimeScreen();
        }
        else
        {
            printf("Failed to save time\n");
        }
    }

    void Settings::saveDateEventCb(lv_event_t *event)
    {
        auto *self = static_cast<Settings *>(lv_event_get_user_data(event));

        uint16_t day =
            lv_roller_get_selected(self->dayRoller) + 1;

        uint16_t month =
            lv_roller_get_selected(self->monthRoller);

        uint16_t year =
            lv_roller_get_selected(self->yearRoller) + 2024;

        struct tm now{};

        rtc_get_time(&now);

        now.tm_mday = day;
        now.tm_mon  = month;
        now.tm_year = year - 1900;

        if (rtc_set_time(&now) == ESP_OK)
        {
            printf("Date saved %02d.%02d.%04d\n",
                day,
                month + 1,
                year);

            self->showDateTimeScreen();
        }
        else
        {
            printf("Failed to save date\n");
        }
    }
}