package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.os.Messenger;

public class MainActivity extends AppCompatActivity {
    public static boolean is_animate = false;
    public int start = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
    public void onClickStart(View view){
        is_animate=true;
        Intent intent = new Intent(this, AnimationService.class);
        MyGLSurfaceView glview = (MyGLSurfaceView) findViewById(R.id.glView);
        Messenger angleReceiver = glview.getAngleReceiver();
        intent.putExtra("ValueMessenger", angleReceiver);
        intent.putExtra("StartValue", 0.0f);
        intent.putExtra("EndValue", 360.0f);
        intent.putExtra("IncValue", 1.0f);
        intent.putExtra("UpdateTime", 100.0f);
        startService(intent);

    }

    public void onClickStop(View view){
    //    MyGLSurfaceView s_glview = (MyGLSurfaceView) findViewById(R.id.glView);
    //    s_glview.onPause();
        is_animate = false;
    }
}