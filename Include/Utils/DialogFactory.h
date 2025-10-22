#ifndef DIALOGFACTORY_H
#define DIALOGFACTORY_H
#include <QDialog>
#include <QString>
#include <functional>
#include <unordered_map>

class DialogFactory {
public:
    using Creator = std::function<QDialog*()>;

    static void registerDialogs();
    static QDialog* create(const QString& key);

private:
    static std::unordered_map<QString, Creator> registry;
};
#endif