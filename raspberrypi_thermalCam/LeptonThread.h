#ifndef TEXTTHREAD
#define TEXTTHREAD

#include <ctime>
#include <stdint.h>
#include <string>
#include <QThread>
#include <QtCore>
#include <QPixmap>
#include <QImage>

#define PACKET_SIZE 164
#define PACKET_SIZE_UINT16 (PACKET_SIZE/2)
#define PACKETS_PER_FRAME 60
#define FRAME_SIZE_UINT16 (PACKET_SIZE_UINT16*PACKETS_PER_FRAME)
using namespace std;

class LeptonThread : public QThread
{
  Q_OBJECT;

public:
  LeptonThread();
  ~LeptonThread();

  void run();
  void ServoBlaster(std::string mvDistance, int servoNumber);
  void tempWarning();
  double LeptonAuxTemp();
  //bool enable();
  //bool disable();
  string convertToString(int passedValue);
  bool firePresence(bool fireSignal);

public slots:
  void performFFC();
  void  showTemp();

signals:
  void updateText(QString);
  void updateImage(QImage);

private:

  QImage myImage;

  uint8_t result[PACKET_SIZE*PACKETS_PER_FRAME];
  uint16_t *frameBuffer;

};

#endif
