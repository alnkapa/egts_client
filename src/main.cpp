#include "mainwindow.h"
#include <QApplication>
#include <QCommandLineParser>

int
main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Создание парсера командной строки
    QCommandLineParser parser;
    parser.setApplicationDescription("Пример приложения на Qt");
    parser.addHelpOption();

    // Опции командной строки
    parser.addOption(QCommandLineOption("imei", "IMEI устройства", "imei"));
    parser.addOption(QCommandLineOption("host", "Хост", "host"));
    parser.addOption(QCommandLineOption("port", "Порт", "port"));
    parser.addOption(QCommandLineOption("file", "Файл", "file"));

    // Парсинг аргументов
    parser.process(a);

    // Получение значений
    QString imei = parser.value("imei");
    QString host = parser.value("host");
    int port = parser.value("port").toInt();
    QString file = parser.value("file");

    MainWindow w;

    // Проверка, заданы ли параметры через командную строку
    if (imei.isEmpty() || host.isEmpty() || port == 0 || file.isEmpty())
    {
        // Если параметры не заданы, открываем диалог
        w.requestConnectionDetails();
    }
    else
    {
        // Если параметры заданы, устанавливаем их в главное окно
        w.setConnectionDetails(imei, host, port, file);
    }
    w.show();
    return a.exec();
}
