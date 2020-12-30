/*
* Copyright (C) 2019 ~ 2020 Uniontech Software Technology Co.,Ltd.
*
* Author:     leiyu <leiyu@uniontech.com>
*
* Maintainer: leiyu <leiyu@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "ut_document.h"

#define private public
#define protected public

#include "document/DjVuModel.h"
#include "document/PDFModel.h"
#include "Application.h"

#undef private
#undef protected

#include <QTimer>
#include <DApplicationSettings>
#include <DLog>
#include <QDebug>

ut_document::ut_document()
{
}

ut_document::~ut_document()
{

}

void ut_document::SetUp()
{
    ut_application::SetUp();
}

void ut_document::TearDown()
{

}

#ifdef UT_DOCUMENT_TEST
TEST_F(ut_document, DJVUTest)
{
    deepin_reader::Document *doc = deepin_reader::DjVuDocument::loadDocument(filePath(UT_FILE_DJVU, "DJVUTest"));
    if (nullptr == doc)
        GTEST_FAIL();

    deepin_reader::Page *page = doc->page(0);
    EXPECT_EQ(page == nullptr, false);

    doc->pageCount();
    doc->saveFilter();
    doc->outline();
    doc->properties();
    doc->label(0);

    page->sizeF();

    QTimer::singleShot(1, [page] {
        page->render(500, 500);
        page->getLinkAtPoint(QPointF(0, 0));
        page->text(QRectF(0, 0, 100, 100));
        page->cachedText(QRectF(0, 0, 100, 100));
        page->search("1", true, true);
        page->formFields();
        page->words();
        page->canAddAndRemoveAnnotations();
        page->annotations();
        page->addIconAnnotation(QRectF(0, 0, 10, 10), "test");
        page->addHighlightAnnotation(QList<QRectF>() << QRectF(0, 0, 20, 20), "test", QColor(Qt::red));
        page->updateAnnotation(nullptr, "test", QColor(Qt::red));
        page->mouseClickIconAnnot(QPointF(0, 0));
        page->moveIconAnnotation(nullptr, QRectF(0, 0, 100, 100));
        page->removeAnnotation(nullptr);
        delete page;
    });

    QFile(filePath(UT_FILE_DJVU_SAVE, "DJVUTest")).remove();
    QFile(filePath(UT_FILE_DJVU_SAVEAS, "DJVUTest")).remove();
    QFile(filePath(UT_FILE_DJVU, "DJVUTest")).copy(filePath(UT_FILE_DJVU_SAVE, "DJVUTest"));
    deepin_reader::Document *saveDoc = deepin_reader::DjVuDocument::loadDocument(filePath(UT_FILE_DJVU_SAVE, "DJVUTest"));
    if (nullptr == saveDoc)
        GTEST_FAIL();
    saveDoc->saveAs(filePath(UT_FILE_DJVU_SAVEAS, "DJVUTest"));
    saveDoc->save();
    QFile(filePath(UT_FILE_DJVU_SAVE, "DJVUTest")).remove();
    QFile(filePath(UT_FILE_DJVU_SAVEAS, "DJVUTest")).remove();
    delete saveDoc;

    exec(1);
}

TEST_F(ut_document, PDFTest)
{
    EXPECT_EQ(deepin_reader::PDFDocument::tryLoadDocument(filePath(UT_FILE_PDF, "PDFTest"), ""), 0);

    deepin_reader::Document *doc = deepin_reader::PDFDocument::loadDocument(filePath(UT_FILE_PDF, "PDFTest"), "");
    if (nullptr == doc) {
        GTEST_FAIL();
    }

    deepin_reader::Page *page = doc->page(0);
    EXPECT_EQ(page == nullptr, false);

    deepin_reader::Link link;
    EXPECT_EQ(link.isValid(), false);

    deepin_reader::SearchResult searchResult;

    deepin_reader::Word word("test", QRectF(0, 0, 100, 100));
    word.wordBoundingRect();

    deepin_reader::Section section;

    QTimer::singleShot(1, [doc, page] {
        doc->pageCount();
        doc->outline();
        doc->properties();
        doc->label(0);
        doc->saveFilter();

        page->sizeF();
        page->render(500, 500);
        page->getLinkAtPoint(QPointF(0, 0));
        page->text(QRectF(0, 0, 100, 100));
        page->cachedText(QRectF(0, 0, 100, 100));
        page->search("1", true, true);
        page->formFields();
        page->words();

        QList< deepin_reader::Annotation * > annots = page->annotations();
        deepin_reader::Annotation *annot = annots.value(0);
        if (nullptr != annot)
        {
            annot->ownAnnotation();
            annot->contents();
            annot->boundary();

            page->addIconAnnotation(QRectF(0, 0, 10, 10), "test");
            page->addHighlightAnnotation(QList<QRectF>() << QRectF(0, 0, 20, 20), "test", QColor(Qt::red));
            page->updateAnnotation(annot, "test", QColor(Qt::red));
            page->mouseClickIconAnnot(QPointF(0, 0));
            page->moveIconAnnotation(annot, QRectF(0, 0, 100, 100));

            //page->removeAnnotation(annot);        //崩溃
            delete page;
        } else
            GTEST_FAIL();
    });

    QFile(filePath(UT_FILE_PDF_SAVE, "PDFTest")).remove();
    QFile(filePath(UT_FILE_PDF_SAVEAS, "PDFTest")).remove();
    EXPECT_EQ(QFile(filePath(UT_FILE_PDF, "PDFTest")).copy(filePath(UT_FILE_PDF_SAVE, "PDFTest")), true);

    deepin_reader::Document *saveDoc = deepin_reader::PDFDocument::loadDocument(filePath(UT_FILE_PDF_SAVE, "PDFTest"), "");
    if (nullptr != saveDoc) {
        saveDoc->save();
        saveDoc->saveAs(filePath(UT_FILE_PDF_SAVEAS, "PDFTest"));
    }

    QFile(filePath(UT_FILE_PDF_SAVE, "PDFTest")).remove();
    QFile(filePath(UT_FILE_PDF_SAVEAS, "PDFTest")).remove();

    delete saveDoc;

    a->handleQuitAction();      //次数需要最后一个case调用，提前调用会导致线程永久性关闭

    exec();
}

#endif
