package com.example.canvas;

import android.graphics.Color;
import android.view.MotionEvent;
import android.view.View;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.content.Context;
import android.graphics.Paint;
import android.graphics.Path;
import java.util.ArrayList;
public class SingleTouchView extends View{

    public static Paint paint = new Paint();
    public static Path path = new Path();
    public static ArrayList<Path> pathlist= new ArrayList<Path>();
    public static ArrayList<Integer> color= new ArrayList<Integer>();
    public static int current_color = Color.BLACK;
    public SingleTouchView(Context context, AttributeSet attrs){
        super(context, attrs);
        paint.setStrokeWidth(10.0f);
        paint.setColor(current_color);
        paint.setStyle(Paint.Style.STROKE);
        paint.setStrokeJoin(Paint.Join.ROUND);
        paint.setAntiAlias(true);
    }

    protected void onDraw(Canvas canvas){
        for(int i=0;i<pathlist.size();i++){
            paint.setColor(color.get(i));
            canvas.drawPath(pathlist.get(i),paint);
            invalidate();
        }
    }

    public void setColor(int c){
        paint.setColor(c);
        path = new Path();
        current_color=(paint.getColor());
        invalidate();
    }

    public void clearPath(){

        pathlist.clear();
        color.clear();
        path.reset();
        invalidate();
    }
    public boolean onTouchEvent(MotionEvent event){
        float eventX = event.getX();
        float eventY = event.getY();

        switch(event.getAction()){
            case MotionEvent.ACTION_DOWN:
                path.moveTo(eventX,eventY);
                break;
            case MotionEvent.ACTION_MOVE:
                path.lineTo(eventX,eventY);
                pathlist.add(path);
                color.add(current_color);
                break;
            case MotionEvent.ACTION_UP:
                break;
            default:
                return false;
        }
        invalidate();
        return true;
    }
}
