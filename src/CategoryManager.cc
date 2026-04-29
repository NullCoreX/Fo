#include "CategoryManager.h"

CategoryManager::CategoryManager() {
    initializeExtensions();
}

CategoryManager::~CategoryManager() {
    // nothing to clean up
}

void CategoryManager::initializeExtensions() {
    audioExts = {"mp3", "wav", "flac", "aac", "ogg"};
    imageExts = {"jpg", "jpeg", "png", "gif", "bmp", "svg"};
    videoExts = {"mp4", "avi", "mkv", "mov", "wmv"};
    documentExts = {"pdf", "doc", "docx", "txt", "xlsx", "pptx"};
}

CategoryManager& CategoryManager::instance() {
    static CategoryManager manager;
    return manager;
}

QString CategoryManager::getCategory(const QString &extension) const {
    QString ext = extension.toLower();

    if (audioExts.contains(ext)) return "Audio";
    if (imageExts.contains(ext)) return "Images";
    if (videoExts.contains(ext)) return "Videos";
    if (documentExts.contains(ext)) return "Documents";
    return "Others";
}

QStringList CategoryManager::getFiltersForCategories(bool all, bool audio, bool images,
                                                     bool videos, bool documents) const {
    QStringList filters;

    if (all || audio) {
        for (const QString& ext : audioExts) filters << "*." + ext;
    }
    if (all || images) {
        for (const QString& ext : imageExts) filters << "*." + ext;
    }
    if (all || videos) {
        for (const QString& ext : videoExts) filters << "*." + ext;
    }
    if (all || documents) {
        for (const QString& ext : documentExts) filters << "*." + ext;
    }

    return filters;
}

QStringList CategoryManager::getAllExtensions() const {
    return audioExts + imageExts + videoExts + documentExts;
}
