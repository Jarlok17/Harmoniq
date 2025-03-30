#pragma once

#include "../brushes/BrushInfo.hpp"
#include "../configs/precompiled.hpp"

namespace harmoniq {

class ToolWrapper : public QObject
{
        Q_OBJECT
    public:
        enum class ToolType {
            None,
            Brush,
            Eraser,
            Fill,
            Move
        };
        Q_ENUM(ToolType)
};

using ToolType = ToolWrapper::ToolType;

class ToolHandler
{
    public:
        virtual ~ToolHandler();

        virtual void press(QMouseEvent *event) = 0;
        virtual void move(QMouseEvent *event) = 0;
        virtual void release(QMouseEvent *event) = 0;
        virtual void setImage(QImage &image) = 0;
        virtual void setBrush(const brushes::Brush &brush) = 0;
};

class BrushTool : public ToolHandler
{
    public:
        BrushTool() {}

        void press(QMouseEvent *event) override
        {
            isDrawing = true;
            m_brush.position = QVector2D(event->position());
            qDebug() << "Brush press at" << event->position();
        }
        void move(QMouseEvent *event) override
        {
            qDebug() << "BRUSH IMAGE: " << canvas;
            if (!isDrawing)
                return;

            QPainter painter(canvas);
            painter.setPen(QPen(m_brush.color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(QPointF(m_brush.position.x(), m_brush.position.y()), event->position());
            m_brush.position = QVector2D(event->position());
            qDebug() << "Brush move at" << event->position();
        }
        void release(QMouseEvent *event) override
        {
            Q_UNUSED(event);
            isDrawing = false;
            qDebug() << "Brush release";
        }
        void setBrush(const brushes::Brush &brush) override { m_brush = brush; }
        void setImage(QImage &image) override { canvas = &image; };

    private:
        brushes::Brush m_brush;
        QImage *canvas;
        bool isDrawing = false;
};

class EraseTool : public ToolHandler
{
    public:
        EraseTool() {}
        void press(QMouseEvent *event) override {}
        void move(QMouseEvent *event) override {}
        void release(QMouseEvent *event) override {}
        void setImage(QImage &image) override {}
        void setBrush(const brushes::Brush &brush) override { m_brush = brush; }

    private:
        QImage *canvas;
        brushes::Brush m_brush;
        bool isErasing = false;
};

} // namespace harmoniq
