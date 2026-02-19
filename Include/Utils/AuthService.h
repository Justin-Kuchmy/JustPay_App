#ifndef AUTHSERVICE_H
#define AUTHSERVICE_H
#include <QFile>
#include <QDir>
#include "../Include/Models/DataObjects.h"
#include <QFile>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <QTextStream>
#include <QLineEdit>
#include <QJsonObject>
#include <QJsonDocument>

class AuthService
{
private:
    AuthService *service;

    explicit AuthService();
    ~AuthService() noexcept;
    AuthService(const AuthService &) = delete;
    AuthService &operator=(const AuthService &) = delete;

    QFile file;
    EmailCrudentials emailCrudentials;

public:
    static AuthService &getInstance()
    {
        static AuthService instance;
        return instance;
    }

    EmailCrudentials createOrReadSettingsFile();
    void writeJsonFile(EmailCrudentials emailCrudentials);
    EmailCrudentials readJsonFile();
};

#endif