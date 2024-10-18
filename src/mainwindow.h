#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

class ConnectionDialog : public QDialog
{
    Q_OBJECT

  public:
    ConnectionDialog(QWidget *parent = nullptr);
    QString
    getImei() const;
    QString
    getHost() const;
    int
    getPort() const;

  private:
    QLineEdit *imeiInput;
    QLineEdit *hostInput;
    QLineEdit *portInput;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void
    setConnectionDetails(const QString &imei, const QString &host, int port, const QString &file);
    void
    requestConnectionDetails();

  private slots:
    void
    onFileInput();
    void
    logMessage(const QString &message);

  private:
    QTextEdit *logOutput;
    QString imei;
    QString host;
    int port;
};

#endif // MAINWINDOW_H
