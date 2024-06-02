#include "mainwindow.h"

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QTimerEvent>

#include "ui_mainwindow.h"

#define GL_SILENCE_DEPRECATION

MainWindow::MainWindow(QWidget *parent)
    : QOpenGLWidget(parent), ui(new Ui::MainWindow) {
  setGeometry(300, 75, 800, 600);

  ui->setupUi(this);
  settings_load = 1;
  default_Args();
  readSettings();
}

MainWindow::~MainWindow() {
  writeSettings();
  delete ui;
  if (struct_mt) {
    remove_matrix(&struct_mt->matrix_of_vertexes);
    for (int i = 1; i <= struct_mt->count_of_facets + 1; i++) {
      free(struct_mt->poligons[i].vertex);
    }
    free(struct_mt->poligons);
    free(struct_mt);
  }
}

void MainWindow::on_line_size_spinBox_valueChanged(int new_val) {
  if (new_file == 2) {
    line_size = new_val;
    resizeGL();
  }
}

void MainWindow::on_line_size_plus_clicked() {
  if (new_file == 2) {
    int val = ui->line_size_spinBox->value();
    ui->line_size_spinBox->setValue(val + 1);
  }
}

void MainWindow::on_line_size_minus_clicked() {
  if (new_file == 2) {
    int val = ui->line_size_spinBox->value();
    ui->line_size_spinBox->setValue(val - 1);
  }
}

void MainWindow::on_dial_line_type_valueChanged(int new_val) {
  if (new_file == 2) {
    line_type = new_val;
    resizeGL();
  }
}

void MainWindow::on_dial_dot_type_valueChanged(int new_val) {
  if (new_file == 2) {
    dot_type = new_val;
    resizeGL();
  }
}

void MainWindow::on_dot_size_spinBox_valueChanged(int new_val) {
  if (new_file == 2) {
    dot_size = new_val;
    resizeGL();
  }
}

void MainWindow::on_dot_size_plus_clicked() {
  if (new_file == 2) {
    int val = ui->dot_size_spinBox->value();
    ui->dot_size_spinBox->setValue(val + 1);
  }
}

void MainWindow::on_dot_size_minus_clicked() {
  if (new_file == 2) {
    int val = ui->dot_size_spinBox->value();
    ui->dot_size_spinBox->setValue(val - 1);
  }
}

void MainWindow::on_dial_color_valueChanged(int new_val) {
  color_type = new_val;
  switch (color_type) {
    case 0:
      ui->Slider_Blue->setValue(fon_b);
      ui->Slider_Red->setValue(fon_r);
      ui->Slider_Green->setValue(fon_g);
      break;
    case 1:
      ui->Slider_Blue->setValue(line_b);
      ui->Slider_Red->setValue(line_r);
      ui->Slider_Green->setValue(line_g);
      break;

    case 2:
      ui->Slider_Blue->setValue(point_b);
      ui->Slider_Red->setValue(point_r);
      ui->Slider_Green->setValue(point_g);
      break;
  }
}

void MainWindow::on_Slider_Blue_valueChanged(int new_val) {
  if (new_file == 2) {
    double val = new_val;
    int type = ui->dial_color->value();
    switch (type) {
      case 0:
        fon_b = val;
        break;
      case 1:
        line_b = val;
        break;

      case 2:
        point_b = val;
        break;
    }
    resizeGL();
  }
}

void MainWindow::on_Slider_Green_valueChanged(int new_val) {
  if (new_file == 2) {
    double val = new_val;
    //        double trans = val/1000;
    int type = ui->dial_color->value();
    switch (type) {
      case 0:
        fon_g = val;
        break;
      case 1:
        line_g = val;
        break;

      case 2:
        point_g = val;
        break;
    }
    resizeGL();
  }
}

void MainWindow::on_Slider_Red_valueChanged(int new_val) {
  if (new_file == 2) {
    double val = new_val;
    //        double trans = val/1000;
    int type = ui->dial_color->value();
    switch (type) {
      case 0:
        fon_r = val;
        break;
      case 1:
        line_r = val;
        break;

      case 2:
        point_r = val;
        break;
    }
    resizeGL();
  }
}

void MainWindow::on_zoom_spinBox_valueChanged(double new_val) {
  if (new_file == 2) {
    double to_Zoom = new_val / cur_Zoom;
    cur_Zoom = new_val;

    scale_matrix(&struct_mt->matrix_of_vertexes, to_Zoom,
                 struct_mt->count_of_vertex);
    resizeGL();
  }
}

void MainWindow::on_zoom_minus_clicked() {
  if (new_file == 2) {
    double val = ui->zoom_spinBox->value();
    ui->zoom_spinBox->setValue(val - 0.1);
  }
}

void MainWindow::on_zoom_plus_clicked() {
  if (new_file == 2) {
    double val = ui->zoom_spinBox->value();
    ui->zoom_spinBox->setValue(val + 0.1);
  }
}

void MainWindow::on_plus_move_button_clicked() {
  double val = ui->move_spinBox->value();
  if (new_file == 2) {
    if (ui->x_move_checkbox->checkState() == Qt::Checked) {
      move_matrix(&struct_mt->matrix_of_vertexes, val, 1,
                  struct_mt->count_of_vertex);
      cur_Xmove += val;
      resizeGL();
    }
    if (ui->y_move_checkbox->checkState() == Qt::Checked) {
      move_matrix(&struct_mt->matrix_of_vertexes, val, 2,
                  struct_mt->count_of_vertex);
      cur_Ymove += val;
      resizeGL();
    }
    if (ui->z_move_checkbox->checkState() == Qt::Checked) {
      move_matrix(&struct_mt->matrix_of_vertexes, val, 3,
                  struct_mt->count_of_vertex);
      cur_Zmove += val;
      resizeGL();
    }
  }
}

void MainWindow::on_minus_move_button_clicked() {
  double val = ui->move_spinBox->value();
  if (new_file == 2) {
    if (ui->x_move_checkbox->checkState() == Qt::Checked) {
      move_matrix(&struct_mt->matrix_of_vertexes, -val, 1,
                  struct_mt->count_of_vertex);
      cur_Xmove -= val;
      resizeGL();
    }
    if (ui->y_move_checkbox->checkState() == Qt::Checked) {
      move_matrix(&struct_mt->matrix_of_vertexes, -val, 2,
                  struct_mt->count_of_vertex);
      cur_Ymove -= val;
      resizeGL();
    }
    if (ui->z_move_checkbox->checkState() == Qt::Checked) {
      move_matrix(&struct_mt->matrix_of_vertexes, -val, 3,
                  struct_mt->count_of_vertex);
      cur_Zmove -= val;
      resizeGL();
    }
  }
}

void MainWindow::on_plus_rotation_button_clicked() {
  double val = ui->rotation_spinBox->value();
  if (new_file == 2) {
    if (ui->x_rotate_checkbox->checkState() == Qt::Checked) {
      xGradus = val;
      cur_Xgradus -= 360 - (360 + val);
      cur_Xgradus = fmod(cur_Xgradus, 360.0);
      resizeGL();
    }
    if (ui->y_rotate_checkbox->checkState() == Qt::Checked) {
      yGradus = val;
      cur_Ygradus -= 360 - (360 + val);
      cur_Ygradus = fmod(cur_Ygradus, 360.0);
      resizeGL();
    }
    if (ui->z_rotate_checkbox->checkState() == Qt::Checked) {
      zGradus = val;
      cur_Zgradus -= 360 - (360 + val);
      cur_Zgradus = fmod(cur_Zgradus, 360.0);
      resizeGL();
    }
  }
}

void MainWindow::on_minus_rotation_button_clicked() {
  double val = ui->rotation_spinBox->value();
  if (new_file == 2) {
    if (ui->x_rotate_checkbox->checkState() == Qt::Checked) {
      xGradus = -val;
      cur_Xgradus += (360 - val);
      cur_Xgradus = fmod(cur_Xgradus, 360.0);
      resizeGL();
    }
    if (ui->y_rotate_checkbox->checkState() == Qt::Checked) {
      yGradus = -val;
      cur_Ygradus += (360 - val);
      cur_Ygradus = fmod(cur_Ygradus, 360.0);
      resizeGL();
    }
    if (ui->z_rotate_checkbox->checkState() == Qt::Checked) {
      zGradus = -val;
      cur_Zgradus += (360 - val);
      cur_Zgradus = fmod(cur_Zgradus, 360.0);
      resizeGL();
    }
  }
}

void MainWindow::on_open_clicked() {
  QString str = QFileDialog::getOpenFileName(this, "Выбрать файл", "",
                                             "Object files (*.obj)");
  if (!str.isEmpty()) {
    if (struct_mt) {
      remove_matrix(&struct_mt->matrix_of_vertexes);
      for (int i = 1; i <= struct_mt->count_of_facets + 1; i++) {
        free(struct_mt->poligons[i].vertex);
      }
      free(struct_mt->poligons);
      free(struct_mt);
    }

    last_part = str.section('/', -1);
    ui->name_file->setText("Name: " + last_part);

    sprintf(file_name, "%s",
            (const char *)((QByteArray)(str.toLatin1()).data()));

    default_Args();
    //    writeSettings();

    resizeGL();

  } else if (new_file == 0) {
    ui->name_file->setText("Name: File Not Selected");
    ui->vershins->setText("Number of vertices: 0");
    ui->rebras->setText("Number of edges: 0");
  }
}

void MainWindow::resizeGL() {
  if (new_file == 1) {
    if (settings_load) {
      readFileName();
      settings_load = 0;
    }

    struct_mt = pars_obj_file(file_name);
    new_file = 2;
    ui->vershins->setText("Number of vertices: " +
                          QString::number(struct_mt->count_of_vertex));
  }
  glMatrixMode(GL_PROJECTION);  // устанавливает текущей проекционную матрицу
  glLoadIdentity();  // присваивает проекционной матрице единичную матрицу
  repaint();
}

void MainWindow::initializeGL()  // инициализация
{
  glEnable(GL_DEPTH_TEST);  // устанавливает режим проверки глубины объектов
  glShadeModel(GL_FLAT);  // отключает режим сглаживания цветов
}

void MainWindow::paintGL()  // рисование
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(fon_r / 255.0f, fon_g / 255.0f, fon_b / 255.0f, 0.0f);
  glLoadIdentity();
  if (perspective == 0) {
    glOrtho(-75.0, 75.0, -75.0, 75.0, -175.0, 125.0);
    glTranslatef(25, 0, 0);
    glScalef(3.5, 5, 5);
  } else if (perspective == 1) {
    float fov = 60.0 * M_PI / 180;  // 60 угол в градусах
    float heapHeight = 3 / (2 * tan(fov / 2));
    glFrustum(-1.0, 1.0, -1.0, 1.0, heapHeight, 50.00);  // параметры видимости
    glTranslatef(0.4, 0, -heapHeight * 3);
    glScalef(0.7, 1, 1);
  }
  //  writeSettings();
  if (xGradus) rotate_matrix(&struct_mt->matrix_of_vertexes, xGradus, 1);
  if (yGradus) rotate_matrix(&struct_mt->matrix_of_vertexes, yGradus, 2);
  if (zGradus) rotate_matrix(&struct_mt->matrix_of_vertexes, zGradus, 3);
  xGradus = 0, yGradus = 0, zGradus = 0;

  if (new_file == 2) {
    drawFigure_func();
  }
}

void MainWindow::drawFigure_func() {
  if (line_type == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0X00FF);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }
  glLineWidth(line_size);
  int rebra = 0;
  for (int i = 1; i <= struct_mt->count_of_facets; i++) {
    glBegin(GL_LINE_LOOP);

    glColor3d(line_r / 255.0f, line_g / 255.0f, line_b / 255.0f);
    for (int j = 0; j < struct_mt->poligons[i].count_vertex_in_facets; j++) {
      x = struct_mt->matrix_of_vertexes
              .matrix[struct_mt->poligons[i].vertex[j]][0];
      y = struct_mt->matrix_of_vertexes
              .matrix[struct_mt->poligons[i].vertex[j]][1];
      z = struct_mt->matrix_of_vertexes
              .matrix[struct_mt->poligons[i].vertex[j]][2];
      glVertex3d(x, y, z);
    }
    glEnd();
  }
  rebra = struct_mt->count_of_vertex + struct_mt->count_of_facets - 2;
  ui->rebras->setText("Number of edges: " + QString::number(rebra));

  glPointSize(dot_size);

  dot_type = ui->dial_dot_type->value();
  if (dot_type == 2) {
    glEnable(GL_POINT_SMOOTH);
  } else {
    glDisable(GL_POINT_SMOOTH);
  }

  if (dot_type > 0) {
    glBegin(GL_POINTS);
    glColor3d(point_r / 255.0f, point_g / 255.0f, point_b / 255.0f);

    for (int i = 1; i < struct_mt->count_of_vertex + 1; i++) {
      x = struct_mt->matrix_of_vertexes.matrix[i][0];
      y = struct_mt->matrix_of_vertexes.matrix[i][1];
      z = struct_mt->matrix_of_vertexes.matrix[i][2];
      glVertex3d(x, y, z);
    }
    glEnd();
  }
}

void MainWindow::default_Args() {
  new_file = 1;
  xGradus = 0.0, yGradus = 0.0, zGradus = 0.0;
  cur_Xgradus = 0, cur_Ygradus = 0, cur_Zgradus = 0;
  std_Zoom = 1.0, cur_Zoom = std_Zoom;
  color_type = 0, dot_type = 1, line_type = 0;
  perspective = 0;
  type_flag = 0;
  fon_r = 253, fon_g = 253, fon_b = 253;
  line_r = 0, line_g = 0, line_b = 0;
  point_r = line_r, point_g = line_g, point_b = line_b;
  dot_size = 1, line_size = 1;
  cur_Xmove = 0.0, cur_Ymove = 0.0, cur_Zmove = 0.0;
  ui->zoom_spinBox->setValue(1.00);
  ui->rotation_spinBox->setValue(0.00);
  ui->move_spinBox->setValue(0.00);
  ui->x_move_checkbox->setChecked(0);
  ui->y_move_checkbox->setChecked(0);
  ui->z_move_checkbox->setChecked(0);
  ui->x_rotate_checkbox->setChecked(0);
  ui->y_rotate_checkbox->setChecked(0);
  ui->z_rotate_checkbox->setChecked(0);
  ui->dial_color->setValue(0);
  ui->Slider_Blue->setValue(fon_b);
  ui->Slider_Green->setValue(fon_g);
  ui->Slider_Red->setValue(fon_r);
  repaint();
  ui->dial_dot_type->setValue(dot_type);
  ui->dial_line_type->setValue(line_type);
  ui->dot_size_spinBox->setValue(dot_size);
  ui->line_size_spinBox->setValue(line_size);
  ui->parallel_radio_but->setChecked(true);
  resizeGL();
}

void MainWindow::writeSettings() {
  QSettings settings("School_project", "3D Viewer");
  settings.beginGroup("data");

  QString name_of_file = file_name;
  settings.setValue("file_name", name_of_file);
  settings.setValue("perspective", perspective);
  settings.setValue("dot_type", dot_type);
  settings.setValue("line_type", line_type);
  settings.setValue("dot_size", dot_size);
  settings.setValue("line_size", line_size);
  settings.setValue("fon_r", fon_r);
  settings.setValue("fon_g", fon_g);
  settings.setValue("fon_b", fon_b);
  settings.setValue("line_r", line_r);
  settings.setValue("line_g", line_g);
  settings.setValue("line_b", line_b);
  settings.setValue("point_r", point_r);
  settings.setValue("point_g", point_g);
  settings.setValue("point_b", point_b);
  settings.setValue("color_type", color_type);
  settings.setValue("last_part", last_part);
  settings.endGroup();
}

void MainWindow::readSettings() {
  QSettings settings("School_project", "3D Viewer");
  settings.beginGroup("data");

  perspective = settings.value("perspective").toInt();
  dot_type = settings.value("dot_type").toInt();
  line_type = settings.value("line_type").toInt();
  dot_size = settings.value("dot_size").toInt();
  line_size = settings.value("line_size").toInt();
  fon_r = settings.value("fon_r").toInt();
  fon_g = settings.value("fon_g").toInt();
  fon_b = settings.value("fon_b").toInt();
  line_r = settings.value("line_r").toInt();
  line_g = settings.value("line_g").toInt();
  line_b = settings.value("line_b").toInt();
  point_r = settings.value("point_r").toInt();
  point_g = settings.value("point_g").toInt();
  point_b = settings.value("point_b").toInt();
  color_type = settings.value("color_type").toInt();
  QString to_view = settings.value("last_part").toString();
  last_part = to_view;
  settings.endGroup();
  ui->name_file->setText("Name: " + to_view);
  ui->Slider_Blue->setValue(fon_b);
  ui->Slider_Green->setValue(fon_g);
  ui->Slider_Red->setValue(fon_r);
  ui->dial_color->setValue(0);
  ui->dial_dot_type->setValue(dot_type);
  ui->dial_line_type->setValue(line_type);
  ui->dot_size_spinBox->setValue(dot_size);
  ui->line_size_spinBox->setValue(line_size);
  if (perspective) ui->central_radio_but->setChecked(true);
}

void MainWindow::readFileName() {
  QSettings settings("School_project", "3D Viewer");
  settings.beginGroup("data");

  QString name_of_file = settings.value("file_name").toString();
  sprintf(file_name, "%s",
          (const char *)((QByteArray)(name_of_file.toLatin1()).data()));
  settings.endGroup();
}

void MainWindow::on_parallel_radio_but_clicked(bool checked) {
  if (new_file == 2 && checked) {
    perspective = 0;
    resizeGL();
  }
}

void MainWindow::on_central_radio_but_clicked(bool checked) {
  if (new_file == 2 && checked) {
    perspective = 1;
    resizeGL();
  }
}

void MainWindow::on_reset_but_clicked() {
  default_Args();
  resizeGL();
}

void MainWindow::SaveToImage() {
  if (ui->checkBoxBMP->isChecked()) {
    QString filename = QFileDialog::getSaveFileName(this, tr("Screen"), "",
                                                    tr("Files ( *.bmp)"));
    if (!filename.isEmpty()) {
      if (filename.right(4) == ".bmp") {
        this->grabFramebuffer().save(filename);
      } else {
        int res = GIFNameError("File extension must be bmp");
        if (res == QMessageBox::Ok) SaveToImage();
      }
    }
  }

  else if (ui->checkBoxJPG->isChecked()) {
    QString filename = QFileDialog::getSaveFileName(this, tr("Screen"), "",
                                                    tr("Files ( *.jpg)"));
    if (!filename.isEmpty()) {
      if (filename.right(4) == ".jpg" || filename.right(5) == ".jpeg") {
        this->grabFramebuffer().save(filename);
      } else {
        int res = GIFNameError("File extension must be jpg or jpeg");
        if (res == QMessageBox::Ok) SaveToImage();
      }
    }
  }
}

void MainWindow::on_ImageButton_clicked() { SaveToImage(); }

void MainWindow::on_checkBoxBMP_stateChanged(int arg1) {
  if (arg1) {
    ui->checkBoxJPG->setChecked(0);
  }
}

void MainWindow::on_checkBoxJPG_stateChanged(int arg1) {
  if (arg1) {
    ui->checkBoxBMP->setChecked(0);
  }
}

int MainWindow::GIFNameError(const char *msg) {
  QMessageBox msgBox(this);
  msgBox.setText(msg);
  msgBox.setInformativeText("OK = repeat \n Cancel = exit to main menu");
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setIcon(QMessageBox::Critical);
  msgBox.setDefaultButton(QMessageBox::Ok);
  return msgBox.exec();
}

void MainWindow::on_ButtonGif_clicked() {
  record = !record;
  if (record) {
    ui->ButtonGif->setText("READY");
    ui->ButtonGif->setEnabled(false);
    reccounter = 0;
    do {
      RecName = QFileDialog::getSaveFileName(this, "Record animation", "./",
                                             ".gif (*.gif)");
      if (!RecName.isEmpty()) {
        if (RecName.right(4) == ".gif") {
          reccounter = 50;
        } else {
          int res = GIFNameError("File extension must be gif");
          if (res == QMessageBox::Ok)
            reccounter = 0;
          else
            reccounter = -1;
        }
      } else {
        reccounter = -1;
      }
    } while (reccounter != 50 && reccounter != -1);
    connect(&recordTimer, SIGNAL(timeout()), this, SLOT(RecordGif()));
    recordTimer.start(100);
  } else {
    ui->ButtonGif->setText("Record\n as\n GIF");
    ui->ButtonGif->setEnabled(true);
    recordTimer.stop();
    disconnect(&recordTimer, nullptr, this, nullptr);
  }
}

void MainWindow::RecordGif() {
  int width = 640;
  int height = 480;
  int delay = 10;
  if (reccounter == 50) {
    ganim.GifBegin(&gwriter, RecName.toLocal8Bit().data(), width, height,
                   delay);
  }
  if (reccounter > 0) {
    QImage frame_scaled =
        this->grabFramebuffer()
            .scaled(width, height, Qt::KeepAspectRatioByExpanding,
                    Qt::SmoothTransformation)
            .convertToFormat(QImage::Format_RGBA8888);
    QRect gifSize(0, 0, width, height);
    gifSize.moveCenter(frame_scaled.rect().center());
    QImage frame = frame_scaled.copy(gifSize);

    ganim.GifWriteFrame(&gwriter, frame.constBits(), width, height, delay);
    --reccounter;
    ui->ButtonGif->setText(QString("Recording,\n do not\n exit... "));
  } else {
    if (reccounter == 0) ganim.GifEnd(&gwriter);
    on_ButtonGif_clicked();
  }
}
