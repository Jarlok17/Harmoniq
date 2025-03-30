#include "Layer.hpp"

namespace harmoniq { namespace layer {

Layer::Layer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
    setFlag(QQuickItem::ItemHasContents);

    qDebug() << "LAYER: " << this;
}

Layer::~Layer() { qDebug() << "DESTROY LAYER: " << this; }

void Layer::updateCanvasSize()
{
    if (width() <= 0 || height() <= 0)
        return;

    if (!m_initialized || m_canvas.size() != QSize(width(), height())) {
        m_canvas = QImage(width(), height(), QImage::Format_ARGB32);
        m_canvas.fill(m_backgroundColor.isValid() ? m_backgroundColor : Qt::transparent);
        m_initialized = true;
        update();
    }
}

void Layer::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);
    updateCanvasSize();
}

void Layer::paint(QPainter *painter)
{
    if (m_visible) {
        if (!m_initialized)
            updateCanvasSize();
        painter->drawImage(0, 0, m_canvas);
    }
}

void Layer::mousePressEvent(QMouseEvent *event)
{
    if (m_toolHandler) {
        m_toolHandler->press(event);
        update();
    }
}

void Layer::mouseMoveEvent(QMouseEvent *event)
{
    if (m_toolHandler) {
        m_toolHandler->move(event);
        update();
    }
}

void Layer::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_toolHandler) {
        m_toolHandler->release(event);
        update();
    }
}

void Layer::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        if (m_initialized) {
            m_canvas.fill(m_backgroundColor);
        }
        emit backgroundColorChanged();
        update();
    }
}

void Layer::setVisibility(const bool &visible)
{
    if (m_visible != visible) {
        m_visible = visible;
        emit visibleUpdated(visible);
        update();
    }
}

void Layer::setCurrentTool(ToolType type)
{
    if (m_currentTool != type) {
        m_currentTool = type;

        switch (m_currentTool) {
        case ToolType::Brush:
            m_toolHandler = std::make_unique<BrushTool>();
            m_toolHandler->setImage(m_canvas);
            m_toolHandler->setBrush(m_brush);

            break;
        case ToolType::Eraser:
            m_toolHandler = std::make_unique<EraseTool>();
            m_toolHandler->setImage(m_canvas);

            break;
        default:
            m_toolHandler.reset();
            break;
        }

        update();
    }
}

QColor Layer::backgroundColor() const { return m_backgroundColor; }

}} // namespace harmoniq::layer
