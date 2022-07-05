package com.example.myapplication;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Messenger;
import android.os.Message;
import android.os.Handler;
import android.util.AttributeSet;

public class MyGLSurfaceView extends GLSurfaceView {

    private final MyGLRenderer mRenderer;

    public MyGLSurfaceView(Context context, AttributeSet attrs) {
        super(context,attrs);

        setEGLContextClientVersion(2);

        mRenderer = new MyGLRenderer();
        setRenderer(mRenderer);

        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    private Handler handler = new Handler(){
        public void handleMessage(Message message){
            mRenderer.setAngle((Float)(message.obj));
            requestRender();
        }
    };

    public Messenger getAngleReceiver(){
        return new Messenger(handler);
    }
    // 1. define and create a handler instance.
    // 2. create a messenger instance with the handler (mAngleReceiver).

}
