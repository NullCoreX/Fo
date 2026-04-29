#ifndef CATEGORYMANAGER_H
#define CATEGORYMANAGER_H

#include <QString>
#include <QStringList>

class CategoryManager {
public:
    static CategoryManager& instance();

    QString getCategory(const QString &extension) const;
    QStringList getFiltersForCategories(bool all, bool audio, bool images,
                                        bool videos, bool documents) const;
    QStringList getAllExtensions() const;

private:
    CategoryManager();
    ~CategoryManager();

    CategoryManager(const CategoryManager&) = delete;
    CategoryManager& operator=(const CategoryManager&) = delete;

    void initializeExtensions();

    QStringList audioExts;
    QStringList imageExts;
    QStringList videoExts;
    QStringList documentExts;
};

#endif // CATEGORYMANAGER_H
