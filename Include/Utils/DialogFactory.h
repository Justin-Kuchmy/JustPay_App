#ifndef DIALOGFACTORY_H
#define DIALOGFACTORY_H
#include <QDialog>
#include <QString>
#include <functional>
#include <unordered_map>

class DialogFactory {
public:
    using Creator = std::function<QWidget*()>;

    static void registerDialogs();
    static QWidget* create(const QString& key);

private:
    static std::unordered_map<QString, Creator> registry;

};
#endif