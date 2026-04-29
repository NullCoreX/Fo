# 📁 File Organizer Pro

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=flat&logo=c%2B%2B)
![Qt](https://img.shields.io/badge/Qt-6-green.svg?style=flat&logo=qt)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)

**File Organizer Pro** is a powerful desktop application built with **Qt6** and **C++17** that automatically organizes your messy folders into neat, categorized subdirectories — with **multi-threading**, **real-time progress logs**, **smart file renaming**, **year-based folder structure**, and **detailed log file generation**.

> 🎯 **Perfect for:** Website administrators, content creators, digital asset managers, developers, students, and anyone who downloads hundreds of files daily.

### 🙌 Who is this for?

- 🧑‍💻 **Website owners & admins** – Organize uploaded assets (images, PDFs, videos) into clean folder structures automatically.
- 📥 **Heavy downloaders** – Tired of your `Downloads` folder looking like a hurricane hit it? This app brings instant order.
- 🎬 **Content creators** – Sort raw footage, audio clips, thumbnails, and scripts into separate folders with unique names.
- 📚 **Students & researchers** – Keep lecture videos, research papers, and presentation slides perfectly organized.
- 🛠️ **Developers** – Quickly sort asset files, documentation, and media resources for your projects.

### 💡 Why use File Organizer Pro?

| Problem | Solution |
|---------|----------|
| ❌ Thousands of files with random names | ✅ Renamed with **date + original name + unique code** (e.g., `2025-04-29 - vacation_photo - aB3Xy7.jpg`) |
| ❌ Manual sorting takes hours | ✅ **One click** – move hundreds of files in seconds |
| ❌ UI freezes during file operations | ✅ **Multi-threaded** – smooth and responsive |
| ❌ Can't track what was moved | ✅ **Live log panel** with detailed status messages + **TXT log file** saved to destination |
| ❌ Website assets scattered everywhere | ✅ Instant categorization into `Images/`, `Videos/`, `Documents/` and more with **year subfolders** |
| ❌ No record of organization history | ✅ **Auto-generated log files** with timestamps, statistics, and full operation details |

### 🚀 Perfect for organizing:

- 📁 **Website upload folders** (WordPress, custom CMS, etc.)
- 📁 **Server download directories**
- 📁 **Cloud sync folders** (Google Drive, Dropbox)
- 📁 **Camera & phone backups**
- 📁 **Project asset folders**
- 📁 **Email attachment dumps**

> 💡 **Pro Tip:** Set your browser's default download folder to a dedicated directory, then run File Organizer Pro once a week to keep everything tidy automatically!

---

## ✨ Features

| Feature | Description |
|---------|-------------|
| 🧠 **Smart Categorization** | Sorts files into `Audio`, `Images`, `Videos` and `Documents` |
| 📅 **Year-Based Folders** | Files are automatically organized into year subfolders based on modification date |
| 📝 **Original Name Preservation** | Renamed files keep their original name: `date - originalName - randomCode.ext` |
| 📄 **Auto Log Files** | Generates timestamped TXT log files with complete operation details in destination folder |
| ⚡ **Multi-threaded** | Uses `QThreadPool` to move files without freezing the UI |
| 🏷️ **Unique Renaming** | Each file gets date + original name + random code (e.g., `2025-04-29 - report - aB3Xy7QzW1kL.pdf`) |
| 📊 **Live Progress** | Real-time progress updates in the log panel |
| 🖱️ **Click&Select** | Click on any path field to open a folder dialog |
| 🎨 **Modern UI** | Gradient buttons, rounded corners, and a clean stylesheet |
| 📝 **Detailed Logs** | Every moved/skipped file is logged with emoji status in both UI and log file |
| ✅ **Selective Sorting** | Choose which file types to organize (Audio, Images, etc.) |
| 🔄 **Safe Operations** | Duplicate filename prevention with unique random codes |

---

## 📸 Screenshot

> *(Add a screenshot of your app here)*  
> 
> 


---

## 🛠️ Requirements

Before building, make sure you have the following installed:

1. **Qt6** (with Widgets module)
2. **C++17** compatible compiler (GCC, Clang, MSVC)
3. **CMake** (version 3.16 or higher)

---

## 🚀 Getting Started

### 1️⃣ Clone the repository

```bash
git clone https://github.com/NullCoreX/Fo.git
cd Fo/
```

### 2️⃣ Build with CMake

```bash
cmake -B build
cmake --build build
```

### 3️⃣ Run the application

```bash
./build/bin/FileOrganizer
```

---

## 🧩 How It Works
1. Select Source Folder – Choose the messy folder you want to organize.
2. Select Destination Folder – Where categorized folders will be created.
3. Choose Categories – Pick Audio, Images, Videos, Documents, or All.
4. Click "Category" – The app scans, moves, and renames files in the background.
5. Watch the Log – Real-time updates on progress and skipped files in the UI.
6. Review Log File – A detailed organize_log_YYYY-MM-DD_HH-MM-SS.txt file is saved in the destination folder.

> 💡 Files are moved, not copied. The original source files will no longer exist in the source folder after moving.

---

## 📁 Output Structure

After running the app, your destination folder will look like this:

```text
Destination/
├── Audio/
│   ├── 2023/
│   │   ├── 2023-04-16 - podcast_intro - aB3Xy7QzW1kL.mp3
│   │   └── ...
│   └── 2024/
│       ├── 2024-08-07 - lecture_recording - pL9mN2qR5tYx.mp3
│       └── ...
├── Images/
│   ├── 2023/
│   │   ├── 2023-11-06 - sunset_photo - xY7zW1kLaB3X.jpg
│   │   └── ...
│   └── 2024/
│       └── ...
├── Videos/
│   ├── 2024/
│   │   ├── 2024-07-21 - presentation - mN2qR5tYxpL9.mp4
│   │   └── ...
│   └── ...
├── Documents/
│   ├── 2023/
│   │   └── ...
│   └── ...
└── organize_log_2025-04-29_14-30-25.txt
```

---

## 🧠 File Type Mapping

| Category       | Extensions                                                                 |
|----------------|----------------------------------------------------------------------------|
| 🎵 **Audio**    | `.mp3`, `.wav`, `.flac`, `.aac`, `.ogg`                                   |
| 🖼️ **Images**   | `.jpg`, `.jpeg`, `.png`, `.gif`, `.bmp`, `.svg`                           |
| 🎬 **Videos**   | `.mp4`, `.avi`, `.mkv`, `.mov`, `.wmv`                                    |
| 📄 **Documents**| `.pdf`, `.doc`, `.docx`, `.txt`, `.xlsx`, `.pptx`                         |

---

## 📋 Log File Format
Each operation generates a timestamped log file in the destination folder with the following format:

```text
═══════════════════════════════════════════
📋 File Organizer Pro - Organization Log
═══════════════════════════════════════════
📅 Date: 2025-04-29 14:30:25
📂 Source: /home/user/Downloads
🎯 Destination: /home/user/Organized
📊 Total files processed: 150
✅ Successfully moved: 145
⚠️ Skipped/Failed: 5
═══════════════════════════════════════════

📝 Detailed Operations:
───────────────────────────────────────────
✅ Moved: photo1.jpg → Images/2024/2024-05-17 - photo1 - aB3Xy7QzW1kL.jpg
✅ Moved: music.mp3 → Audio/2023/2023-12-01 - music - xY7zW1kLaB3X.mp3
⚠️ File not found: missing_file.pdf
...

═══════════════════════════════════════════
🏁 Operation completed at: 2025-04-29 14:30:30
```

---

## ⚙️ Customization

You can easily extend the supported file types by modifying the getCategory() method in fo.cc:
```bash
QStringList audioExts = {"mp3", "wav", "flac", "aac", "ogg", "m4a"};
```

---

## ⭐ Support

If you find this tool useful, please give it a ⭐ on GitHub!
Pull requests and feature suggestions are always welcome.

---
