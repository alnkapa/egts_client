#include "mainwindow.h"
#include <QFileDialog>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

ConnectionDialog::ConnectionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Введите данные подключения");

    imeiInput = new QLineEdit(this);
    hostInput = new QLineEdit(this);
    portInput = new QLineEdit(this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("IMEI:", imeiInput);
    formLayout->addRow("Хост:", hostInput);
    formLayout->addRow("Порт:", portInput);

    QPushButton *okButton = new QPushButton("OK", this);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(okButton);
    setLayout(mainLayout);
}
void
MainWindow::setConnectionDetails(const QString &imei, const QString &host, int port, const QString &file)
{
    logOutput->append("IMEI: " + imei);
    logOutput->append("Хост: " + host);
    logOutput->append("Порт: " + QString::number(port));
    logOutput->append("Файл: " + file);
}

QString
ConnectionDialog::getImei() const
{
    return imeiInput->text();
}

QString
ConnectionDialog::getHost() const
{
    return hostInput->text();
}

int
ConnectionDialog::getPort() const
{
    return portInput->text().toInt();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), logOutput(new QTextEdit(this))
{

    setWindowTitle("My Qt Application");

    // Настройка интерфейса
    logOutput->setReadOnly(true);
    QPushButton *fileButton = new QPushButton("Выбрать файл", this);
    connect(fileButton, &QPushButton::clicked, this, &MainWindow::onFileInput);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(new QLabel("Логи:"));
    layout->addWidget(logOutput);
    layout->addWidget(fileButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

void
MainWindow::requestConnectionDetails()
{
    // Создание диалогового окна для ввода данных
    QDialog dialog(this);
    dialog.setWindowTitle("Введите данные подключения");

    QLineEdit *imeiInput = new QLineEdit(&dialog);
    QLineEdit *hostInput = new QLineEdit(&dialog);
    QLineEdit *portInput = new QLineEdit(&dialog);
    QLineEdit *fileInput = new QLineEdit(&dialog);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("IMEI:", imeiInput);
    formLayout->addRow("Хост:", hostInput);
    formLayout->addRow("Порт:", portInput);
    formLayout->addRow("Файл:", fileInput);

    QPushButton *okButton = new QPushButton("OK", &dialog);
    connect(okButton, &QPushButton::clicked, [&]()
            {
                // Проверка на корректность ввода
                if (imeiInput->text().isEmpty() || hostInput->text().isEmpty() || portInput->text().isEmpty())
                {
                    QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
                    return;
                }

                // Установка значений в главное окно
                QString imei = imeiInput->text();
                QString host = hostInput->text();
                int port = portInput->text().toInt();
                QString file = fileInput->text();

                setConnectionDetails(imei, host, port, file);
                dialog.accept(); // Закрыть диалог
            });

    QVBoxLayout *dialogLayout = new QVBoxLayout;
    dialogLayout->addLayout(formLayout);
    dialogLayout->addWidget(okButton);
    dialog.setLayout(dialogLayout);

    dialog.exec(); // Показать диалог
}

void
MainWindow::onFileInput()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выбрать файл");
    if (!fileName.isEmpty())
    {
        logMessage("Выбран файл: " + fileName);
        // Здесь можно добавить логику для обработки файла
    }
}

void
MainWindow::logMessage(const QString &message)
{
    logOutput->append(message);
}
