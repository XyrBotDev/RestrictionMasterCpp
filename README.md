# RestrictionMasterCpp

RestrictionMasterCpp is a C++17 Telegram bot project.

The project is organized into separate components for command handling, bot management, configuration loading, database access, downloading, uploading, transfers, Telegram integration, and access control.

## Current Project Status

The project currently contains the core C++ project structure and build configuration.

The GitHub Actions C++ build is configured and the project currently builds successfully.

The Telegram runtime integration is still being completed.

---

## Features

### User Commands

- `/start` — Start the bot
- `/help` — Show help menu
- `/login` — Login
- `/logout` — Logout
- `/status` — Show status
- `/cancel` — Cancel an operation

### Customization

- `/setcaption` — Set caption
- `/delcaption` — Delete caption
- `/setthumb` — Set thumbnail
- `/delthumb` — Delete thumbnail
- `/setprefix` — Set prefix
- `/setsuffix` — Set suffix

### User Information

- `/mystats` — Show user statistics
- `/myinfo` — Show user information
- `/history` — Show download history

### Settings

- `/settings` — Open settings
- `/language` — Change language

### Extra

- `/refer` — Referral information
- `/bookmark` — Bookmark content
- `/bookmarks` — Show bookmarks
- `/favorite` — Add favorite
- `/favorites` — Show favorites
- `/share` — Share
- `/feedback` — Send feedback
- `/premium` — Premium information

---

## Admin Commands

### Statistics

- `/stats`
- `/users`
- `/activeusers`
- `/newusers`
- `/topusers`
- `/broadcast`

### User Management

- `/ban`
- `/unban`
- `/banlist`
- `/finduser`
- `/userinfo`

### Premium Management

- `/addpremium`
- `/removepremium`
- `/premiumlist`

### Admin Management

- `/addadmin`
- `/removeadmin`
- `/adminlist`

### Bot Configuration

- `/setfsub`
- `/delfsub`
- `/maintenance`
- `/feedbacks`
- `/sendmsg`
- `/clearlogs`
- `/export`
- `/adminhelp`

### Dummy Channel

- `/dummy`
- `/rdummy`

The dummy-channel functionality is intended for configuring a channel through the bot.

---

## Access Control

The project contains an `AccessControl` component for:

- Owner verification
- Admin verification
- Premium-user verification
- Banned-user verification
- Maintenance mode
- Adding admins
- Removing admins

The owner ID is loaded from the application configuration.

---

## Configuration

The project uses a configuration file based on the example file:

```text
config/config.example.json
