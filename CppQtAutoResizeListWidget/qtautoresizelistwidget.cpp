#include "qtautoresizelistwidget.h"

#include <QObject>
#include <QScrollBar>
#include <QTimer>

QtAutoResizeListWidget::QtAutoResizeListWidget(QWidget *parent)
  : QListWidget(parent),
    m_timer(new QTimer)
{
  QObject::connect(m_timer,&QTimer::timeout,this,&QtAutoResizeListWidget::OnTimer);
  m_timer->setInterval(1);
  m_timer->start();
}

void QtAutoResizeListWidget::addItem(QListWidgetItem *item)
{

  QListWidget::addItem(item);
  this->repaint();
  m_timer->start();
}

void QtAutoResizeListWidget::OnTimer()
{
  if (this->verticalScrollBar()->isVisible())
  {
    const int height = this->height();
    this->setMinimumHeight(height + 1);
  }
  else
  {
    m_timer->stop();
  }
}
