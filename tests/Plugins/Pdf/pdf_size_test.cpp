
/******************************************************************************
 * MODULE     : pdf_size_test_test.cpp
 * COPYRIGHT  : (C) Tangdouer
 *******************************************************************************
 * This software falls under the GNU general public license version 3 or later.
 * It comes WITHOUT ANY WARRANTY WHATSOEVER. For details, see the file LICENSE
 * in the root directory or <http://www.gnu.org/licenses/gpl-3.0.html>.
 ******************************************************************************/

#include "Pdf/pdf_hummus_renderer.hpp"
#include <QtTest/QtTest>

class TestHummusPdfImageSize : public QObject {
  Q_OBJECT

private slots:
  void test_hummus_pdf_image_size ();
};

void
TestHummusPdfImageSize::test_hummus_pdf_image_size () {
  int w= 0, h= 0;
  hummus_pdf_image_size (url ("$TEXMACS_PATH/tests/images/26_2_1.pdf"), w, h);
  QCOMPARE (w, 595);
  QCOMPARE (h, 595);
}

QTEST_MAIN (TestHummusPdfImageSize)
#include "pdf_size_test.moc"