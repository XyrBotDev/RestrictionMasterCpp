#include "../../include/keyboards.hpp"

#include <string>

namespace Keyboards {

std::string mainMenu() {
    return
        "{\"inline_keyboard\":["
            "["
                "{\"text\":\"💎 Premium\",\"callback_data\":\"premium\"},"
                "{\"text\":\"⚙️ Settings\",\"callback_data\":\"settings\"}"
            "],"
            "["
                "{\"text\":\"🤖 Bot List\",\"callback_data\":\"bot_list\"},"
                "{\"text\":\"📊 Statistics\",\"callback_data\":\"bot_stats\"}"
            "]"
        "]}";
}

std::string premiumMenu() {
    return
        "{\"inline_keyboard\":["
            "["
                "{\"text\":\"⬅️ Main Menu\",\"callback_data\":\"main_menu\"}"
            "]"
        "]}";
}

std::string settingsMenu() {
    return
        "{\"inline_keyboard\":["
            "["
                "{\"text\":\"🔔 Notifications\",\"callback_data\":\"settings_notifications\"}"
            "],"
            "["
                "{\"text\":\"🌙 Silent Mode\",\"callback_data\":\"settings_silent\"}"
            "],"
            "["
                "{\"text\":\"🌐 Language\",\"callback_data\":\"settings_language\"}"
            "],"
            "["
                "{\"text\":\"⬅️ Main Menu\",\"callback_data\":\"main_menu\"}"
            "]"
        "]}";
}

std::string botList() {
    return
        "{\"inline_keyboard\":["
            "["
                "{\"text\":\"⬅️ Main Menu\",\"callback_data\":\"main_menu\"}"
            "]"
        "]}";
}

std::string botControl(
    int botNumber
) {
    return
        "{\"inline_keyboard\":["
            "["
                "{\"text\":\"🔧 Maintenance\",\"callback_data\":\"bot_control:" +
                std::to_string(botNumber) +
                "\"}"
            "],"
            "["
                "{\"text\":\"⬅️ Bot List\",\"callback_data\":\"bot_list\"}"
            "]"
        "]}";
}

std::string maintenanceMenu(
    int botNumber
) {
    return
        "{\"inline_keyboard\":["
            "["
                "{\"text\":\"🔧 Turn ON\",\"callback_data\":\"maintenance_on:" +
                std::to_string(botNumber) +
                "\"},"
                "{\"text\":\"✅ Turn OFF\",\"callback_data\":\"maintenance_off:" +
                std::to_string(botNumber) +
                "\"}"
            "],"
            "["
                "{\"text\":\"⬅️ Bot List\",\"callback_data\":\"bot_list\"}"
            "]"
        "]}";
}

std::string dummyRefresh() {
    return
        "{\"inline_keyboard\":["
            "["
                "{\"text\":\"🔄 Refresh\",\"callback_data\":\"dummy_refresh\"}"
            "],"
            "["
                "{\"text\":\"⬅️ Main Menu\",\"callback_data\":\"main_menu\"}"
            "]"
        "]}";
}

std::string rangeDownloadControls() {
    return
        "{\"inline_keyboard\":["
            "["
                "{\"text\":\"⏹ Stop\",\"callback_data\":\"range_stop\"}"
            "]"
        "]}";
}

}
