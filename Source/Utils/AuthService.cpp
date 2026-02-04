#include "Utils/AuthService.h"
using json = nlohmann::json;
std::string base64_decode(const std::string &in);
static void to_json(json &j, const EmailCrudentials &e)
{
    j = json{{"companyEmail", e.companyEmail}, {"appPassword", e.appPassword}};
}

static void from_json(const json &j, EmailCrudentials &e)
{
    j.at("companyEmail").get_to(e.companyEmail);
    j.at("appPassword").get_to(e.appPassword);
}

EmailCrudentials AuthService::createOrReadSettingsFile()
{

    QString fileName = "emailCrudentials.json";
    QString path = QDir::currentPath() + "/Resources/";
    file.setFileName(path + fileName);
    QDir dir;

    if (!dir.exists(path + fileName))
    {
        dir.mkpath(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Could not open the file: " << file.errorString();
            return {};
        }
        file.close();
    }
    else
    {
        return readJsonFile();
    }
    return {};
}

void AuthService::writeJsonFile(EmailCrudentials emailCrudentials)
{

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Could not open the file for Writing:" << file.errorString();
        return;
    }
    emailCrudentials.companyEmail = QString::fromLatin1(QString::fromStdString(emailCrudentials.companyEmail).toUtf8().toBase64()).toStdString();
    emailCrudentials.appPassword = QString::fromLatin1(QString::fromStdString(emailCrudentials.appPassword).toUtf8().toBase64()).toStdString();

    nlohmann::json j = emailCrudentials;

    std::string dump = j.dump();

    QTextStream out(&file);
    QString text = QString::fromStdString(dump);
    out << text;
    file.close();
}

EmailCrudentials AuthService::readJsonFile()
{
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Could not open the file for reading:" << file.errorString();
        return {};
    }

    QTextStream inputFile(&file);
    json j = json::parse(inputFile.readAll().toStdString());
    emailCrudentials = j.get<EmailCrudentials>();
    emailCrudentials.companyEmail = base64_decode(emailCrudentials.companyEmail);
    emailCrudentials.appPassword = base64_decode(emailCrudentials.appPassword);
    file.close();

    return emailCrudentials;
}

std::string base64_decode(const std::string &in)
{
    std::string table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string data;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++)
        T[table[i]] = i;

    std::vector<unsigned char> out;
    int val = 0, valb = -8;
    for (unsigned char c : in)
    {
        if (T[c] == -1)
            break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0)
        {
            out.push_back(char((val >> valb) & 0xFF));
            data += char((val >> valb) & 0xFF);
            valb -= 8;
        }
    }
    return data;
}

AuthService::AuthService() {}

AuthService::~AuthService() {}