SET(SOURCES components/ui_comp_contheader.c
    components/ui_comp.c
    screens/ui_ScrMain.c
    screens/ui_ScrWeather.c
    screens/ui_ScrMusic.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.c
    ui_img_manager.c
    images/ui_img_setting_png.c
    images/ui_img_calendar_png.c
    images/ui_img_weather_png.c
    images/ui_img_fe_png.c
    images/ui_img_album_png.c
    images/ui_img_vedio_png.c
    images/ui_img_music_png.c
    images/ui_img_camera_png.c
    images/ui_img_cloudy_png.c
    images/ui_img_rain_l_png.c
    images/ui_img_thunder_rain_png.c
    images/ui_img_rainfall_png.c
    images/ui_img_humidity_png.c
    images/ui_img_wind_png.c
    images/ui_img_wind_speed_png.c)

add_library(ui ${SOURCES})
