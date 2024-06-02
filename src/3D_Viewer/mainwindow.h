#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QFileDialog>
#include <QMainWindow>
#include <QTimer>

#include "gifanim/gifanim.h"
extern "C" {
#include "transformations.h"
#include "viewer.h"
}

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMatrix4x4>
#include <QMessageBox>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QSettings>
#include <QTimerEvent>
#include <QWidget>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QOpenGLWidget {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void resizeGL();
  void initializeGL();
  void paintGL();
  void drawFigure_func();

 private slots:
  void on_open_clicked();
  void on_zoom_plus_clicked();
  void on_zoom_minus_clicked();
  void on_plus_rotation_button_clicked();
  void on_minus_rotation_button_clicked();
  void on_minus_move_button_clicked();
  void on_plus_move_button_clicked();
  void on_dot_size_minus_clicked();
  void on_dot_size_plus_clicked();
  void on_line_size_plus_clicked();
  void on_line_size_minus_clicked();
  void on_reset_but_clicked();
  void writeSettings();
  void readSettings();
  void readFileName();

  void on_zoom_spinBox_valueChanged(double new_val);
  void on_Slider_Red_valueChanged(int new_val);
  void on_Slider_Green_valueChanged(int new_val);
  void on_Slider_Blue_valueChanged(int new_val);
  void on_dial_color_valueChanged(int new_val);
  void on_dial_dot_type_valueChanged(int new_val);
  void on_dot_size_spinBox_valueChanged(int new_val);
  void on_dial_line_type_valueChanged(int new_val);
  void on_line_size_spinBox_valueChanged(int new_val);

  void default_Args();

  void on_parallel_radio_but_clicked(bool checked);
  void on_central_radio_but_clicked(bool checked);

  // Images
  void SaveToImage();
  void on_ImageButton_clicked();
  void on_checkBoxBMP_stateChanged(int arg1);
  void on_checkBoxJPG_stateChanged(int arg1);
  void RecordGif();
  void on_ButtonGif_clicked();
  int GIFNameError(const char *msg);

 private:
  Ui::MainWindow *ui;
  model *struct_mt = nullptr;
  QString last_part;
  int new_file = 0;
  char file_name[250];
  int color_type = 0, dot_type = 1, line_type = 0, perspective = 0;
  int type_flag = 0, settings_load, flag = 0;
  double x, y, z;
  int fon_r = 253, fon_g = 253, fon_b = 253;
  int line_r = 0, line_g = 0, line_b = 0;
  int point_r = line_r, point_g = line_g, point_b = line_b;
  int dot_size = 1, line_size = 1;
  double xGradus = 0, yGradus = 0, zGradus = 0;
  double cur_Xgradus = 0, cur_Ygradus = 0, cur_Zgradus = 0;
  double new_cur_Zoom, std_Zoom = 1.0, cur_Zoom = std_Zoom;
  double cur_Xmove = 0, cur_Ymove = 0, cur_Zmove = 0;
  // video recording variables
  bool record = false;
  QTimer recordTimer;
  int reccounter;
  QString RecName;
  GifAnim ganim;
  GifWriter gwriter;
  QString appPath;
};
#endif  // MAINWINDOW_H
