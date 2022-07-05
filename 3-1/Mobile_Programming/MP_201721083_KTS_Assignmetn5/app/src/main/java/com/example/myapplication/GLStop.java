package com.example.myapplication;

import android.app.Activity;
import android.opengl.GLSurfaceView;


public class GLStop extends Activity{
    private GLSurfaceView mGLView;

    @Override
    protected void onPause() {
        super.onPause();
        mGLView.onPause();
    }

    protected void onResume() {
        super.onResume();
        mGLView.onResume();
    }
}
