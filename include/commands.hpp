#pragma once

#include <string>

namespace Commands {

// User
std::string start();
std::string help();
std::string login();
std::string logout();
std::string status();
std::string cancel();

// Customization
std::string setCaption();
std::string deleteCaption();
std::string setThumbnail();
std::string deleteThumbnail();
std::string setPrefix();
std::string setSuffix();

// User information
std::string myStats();
std::string myInfo();
std::string history();

// Settings
std::string settings();
std::string language();

// Extra
std::string refer();
std::string bookmark();
std::string bookmarks();
std::string favorite();
std::string favorites();
std::string share();
std::string feedback();
std::string premium();

// Admin
std::string stats();
std::string users();
std::string activeUsers();
std::string newUsers();
std::string topUsers();
std::string broadcast();

std::string ban();
std::string unban();
std::string banList();
std::string findUser();
std::string userInfo();

// Premium management
std::string addPremium();
std::string removePremium();
std::string premiumList();

// Admin management
std::string addAdmin();
std::string removeAdmin();
std::string adminList();

// Bot configuration
std::string setFsub();
std::string deleteFsub();
std::string maintenance();
std::string feedbacks();
std::string sendMessage();
std::string clearLogs();
std::string exportUsers();
std::string adminHelp();

std::string dummy();
std::string removeDummy();

}
