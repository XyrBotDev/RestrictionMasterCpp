#include "../../include/commands.hpp"

namespace Commands {

// User
std::string start() { return "/start"; }
std::string help() { return "/help"; }
std::string login() { return "/login"; }
std::string logout() { return "/logout"; }
std::string status() { return "/status"; }
std::string cancel() { return "/cancel"; }

// Customization
std::string setCaption() { return "/setcaption"; }
std::string deleteCaption() { return "/delcaption"; }
std::string setThumbnail() { return "/setthumb"; }
std::string deleteThumbnail() { return "/delthumb"; }
std::string setPrefix() { return "/setprefix"; }
std::string setSuffix() { return "/setsuffix"; }

// User information
std::string myStats() { return "/mystats"; }
std::string myInfo() { return "/myinfo"; }
std::string history() { return "/history"; }

// Settings
std::string settings() { return "/settings"; }
std::string language() { return "/language"; }

// Extra
std::string refer() { return "/refer"; }
std::string bookmark() { return "/bookmark"; }
std::string bookmarks() { return "/bookmarks"; }
std::string favorite() { return "/favorite"; }
std::string favorites() { return "/favorites"; }
std::string share() { return "/share"; }
std::string feedback() { return "/feedback"; }
std::string premium() { return "/premium"; }

// Admin
std::string stats() { return "/stats"; }
std::string users() { return "/users"; }
std::string activeUsers() { return "/activeusers"; }
std::string newUsers() { return "/newusers"; }
std::string topUsers() { return "/topusers"; }
std::string broadcast() { return "/broadcast"; }

std::string ban() { return "/ban"; }
std::string unban() { return "/unban"; }
std::string banList() { return "/banlist"; }
std::string findUser() { return "/finduser"; }
std::string userInfo() { return "/userinfo"; }

// Premium management
std::string addPremium() { return "/addpremium"; }
std::string removePremium() { return "/removepremium"; }
std::string premiumList() { return "/premiumlist"; }

// Admin management
std::string addAdmin() { return "/addadmin"; }
std::string removeAdmin() { return "/removeadmin"; }
std::string adminList() { return "/adminlist"; }

// Bot configuration
std::string setFsub() { return "/setfsub"; }
std::string deleteFsub() { return "/delfsub"; }
std::string maintenance() { return "/maintenance"; }
std::string feedbacks() { return "/feedbacks"; }
std::string sendMessage() { return "/sendmsg"; }
std::string clearLogs() { return "/clearlogs"; }
std::string exportUsers() { return "/export"; }
std::string adminHelp() { return "/adminhelp"; }

}
