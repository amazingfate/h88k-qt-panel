#include <QApplication>
#include <QScreen>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>
#include <QNetworkInterface>
#include <QDateTime>
#include <fstream>
#include <sstream>

class TFTApp : public QWidget {
    Q_OBJECT
public:
    explicit TFTApp(QWidget *parent = nullptr);
    virtual ~TFTApp() {}  // Virtual destructor for safety.
private slots:
    void updateInfo();
private:
    QLabel *cpuTempLabel;
    QLabel *ipLabel;
    QLabel *dateTimeLabel; // Label for date and time
    QString getCpuTemperature();
    QString getIpAddress();
};

TFTApp::TFTApp(QWidget *parent) : QWidget(parent) {
    // Set the window size to match the TFT screen
    setWindowTitle("TFT Display");
    resize(240, 135);

    // Create labels for CPU temperature, IP address, and date-time
    cpuTempLabel = new QLabel("CPU Temp: -- °C", this);
    ipLabel = new QLabel("IP: --.--.--.--", this);
    dateTimeLabel = new QLabel("Date & Time: --", this);

    // Customize fonts for better readability
    QFont font("Arial", 12, QFont::Bold);
    cpuTempLabel->setFont(font);
    ipLabel->setFont(font);
    dateTimeLabel->setFont(font);

    // Set text color to white and background color to black
    setStyleSheet("background-color: black; color: white;");

    // Align labels to center
    cpuTempLabel->setAlignment(Qt::AlignCenter);
    ipLabel->setAlignment(Qt::AlignCenter);
    dateTimeLabel->setAlignment(Qt::AlignCenter);

    // Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(cpuTempLabel);
    layout->addWidget(ipLabel);
    layout->addWidget(dateTimeLabel);
    setLayout(layout);

    // Update information periodically
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TFTApp::updateInfo);
    timer->start(1000); // Update every second
}

void TFTApp::updateInfo() {
    cpuTempLabel->setText(QString("CPU Temp: %1").arg(getCpuTemperature()));
    ipLabel->setText(QString("IP: %1").arg(getIpAddress()));
    dateTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
}

QString TFTApp::getCpuTemperature() {
    std::ifstream file("/sys/class/thermal/thermal_zone0/temp");
    if (file.is_open()) {
        std::string tempStr;
        std::getline(file, tempStr);
        file.close();
        double tempC = std::stod(tempStr) / 1000.0;
        return QString::number(tempC, 'f', 1) + " °C";
    }
    return "N/A";
}

QString TFTApp::getIpAddress() {
    const auto interfaces = QNetworkInterface::allInterfaces();
    for (const auto &interface : interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack)) {
            for (const auto &entry : interface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    return entry.ip().toString();
                }
            }
        }
    }
    return "N/A";
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Get the list of screens
    QList<QScreen *> screens = QGuiApplication::screens();
    QScreen *targetScreen = nullptr;

    // Identify the screen with resolution 240x135
    for (QScreen *screen : screens) {
        if (screen->size() == QSize(240, 135)) {
            targetScreen = screen;
            break;
        }
    }

    if (!targetScreen) {
        // Exit if the target screen isn't found
        qCritical("No screen with resolution 240x135 found. Exiting.");
        return -1;
    }

    // Create and show the app on the target screen
    TFTApp window;
    QRect screenGeometry = targetScreen->geometry();
    window.move(screenGeometry.topLeft());
    window.showFullScreen();

    return app.exec();
}
#include "main.moc"
