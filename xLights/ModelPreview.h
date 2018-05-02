#ifndef MODELPREVIEW_H
#define MODELPREVIEW_H

#include <memory>

#include <wx/wx.h>

#include "Image.h"
#include "XlightsDrawable.h"
#include "Color.h"
#include "xlGLCanvas.h"

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model;
class PreviewPane;
class LayoutGroup;
class xLightsFrame;
class xlVertex3Accumulator;

class ModelPreview : public xlGLCanvas
{

public:
    ModelPreview(wxPanel* parent);
	ModelPreview(wxPanel* parent,
                 xLightsFrame* xlights_,
                 std::vector<Model*> &models,
                 std::vector<LayoutGroup *> &groups,
                 bool allowSelected,
                 int style = 0,
                 bool allowPreviewChange = false);
	virtual ~ModelPreview();

    // Public Methods
	void InitializePreview(wxString img,int brightness);
    bool StartDrawing(wxDouble pointSize);
    void SetPointSize(wxDouble pointSize);
    void EndDrawing(bool swapBuffers=true);
	void SetCanvasSize(int width,int height);
    void SetVirtualCanvasSize(int width, int height);
    void GetVirtualCanvasSize(int &w, int& h) const
    {
        w = virtualWidth; h = virtualHeight;
    }
    int GetVirtualCanvasHeight() const
    {
        return virtualHeight;
    }
    int GetVirtualCanvasWidth() const
    {
        return virtualWidth;
    }
	void SetbackgroundImage(wxString image);
    const wxString &GetBackgroundImage() const { return mBackgroundImage;}
	void SetBackgroundBrightness(int brightness);
    int GetBackgroundBrightness() const { return mBackgroundBrightness;}
    void SetScaleBackgroundImage(bool b);
    bool GetScaleBackgroundImage() const { return scaleImage; }

	void SetCameraView(int camerax, int cameray, bool latch);
	void SetCameraPos(int camerax, int cameray, bool latch);
    void SetZoomDelta(float delta);

    void Render();
    void Render(const unsigned char *data, bool swapBuffers=true);

    double calcPixelSize(double i);
    void SetModels(std::vector<Model*> &models) {
        previewModels.clear();
        previewModels = models;
        PreviewModels = &previewModels;
    }
    void SetModel(Model* model) { _model = model; }
    std::vector<Model*> &GetModels() {
        if (PreviewModels == nullptr) {
            return previewModels;
        }
        return *PreviewModels;
    }

    void SetPreviewPane(PreviewPane* pane) {mPreviewPane = pane;}
    void SetActive(bool show);
    bool GetActive();
    float GetZoom() { return (is_3d ? zoom : zoom2D); }
    void SetPan(float deltax, float deltay);
    void Set3D(bool value) { is_3d = value; }
    bool Is3D() { return is_3d; }
    glm::mat4& GetViewMatrix() { return ViewMatrix; }
    glm::mat4& GetProjMatrix() { return ProjMatrix; }

	virtual void render(const wxSize& size=wxSize(0,0)) override;

    DrawGLUtils::xlAccumulator &GetAccumulator() {return accumulator;}
protected:
    virtual void InitializeGLCanvas() override;
    virtual bool UsesVertexTextureAccumulator() override {return true;}
    virtual bool UsesVertexColorAccumulator() override {return false;}
    virtual bool UsesVertexAccumulator() override {return false;}
    virtual bool UsesAddVertex() override {return true;}
    virtual bool UsesVertex3Accumulator() {return true;}
    virtual bool UsesVertex3ColorAccumulator() {return true;}

private:
	void render(wxPaintEvent& event);
	void SetOrigin();
	void mouseMoved(wxMouseEvent& event);
	void mouseLeftDown(wxMouseEvent& event);
	void mouseLeftUp(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
    void mouseMiddleDown(wxMouseEvent& event);
    void mouseMiddleUp(wxMouseEvent& event);
    //void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
    void OnPopup(wxCommandEvent& event);
	void drawGrid(float size, float step);

    bool mIsDrawing = false;
    bool mBackgroundImageExists = false;
    wxString mBackgroundImage;
    int  mBackgroundBrightness=100;
    wxDouble mPointSize = 2.0;
    int virtualWidth, virtualHeight;

    Image* image = nullptr;
    bool scaleImage = false;
    xLightsDrawable* sprite;
    std::vector<Model*> previewModels;
    std::vector<Model*> *PreviewModels;
    std::vector<Model*> *HouseModels;
    std::vector<LayoutGroup *> *LayoutGroups;
    bool allowSelected;
    bool allowPreviewChange;
    PreviewPane* mPreviewPane;
    DrawGLUtils::xlAccumulator accumulator;
    Model* _model;
    xLightsFrame* xlights;
    float cameraAngleY;
    float cameraAngleX;
    float cameraDistance;
    float zoom;
    float zoom2D;  // zoom works opposite in 2D
    float panx;
    float pany;
    float panx2D;
    float pany2D;
    float zoom_corrx2D;
    float zoom_corry2D;
    float cameraPosX;
	float cameraPosY;
	DrawGLUtils::xlVertex3Accumulator gridlines;
	DrawGLUtils::xl3Accumulator accumulator3d;
    bool is_3d;
    bool m_mouse_down;
    bool m_wheel_down;
    int m_last_mouse_x, m_last_mouse_y;
    glm::mat4 ViewMatrix;
    glm::mat4 ProjMatrix;

    double currentPixelScaleFactor = 1.0;

    int maxVertexCount;

	DECLARE_EVENT_TABLE()
};


#endif // MODELPREVIEW_H
