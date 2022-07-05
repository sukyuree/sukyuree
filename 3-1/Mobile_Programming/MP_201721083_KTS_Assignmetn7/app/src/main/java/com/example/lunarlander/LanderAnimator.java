package com.example.lunarlander;

import android.animation.AnimatorListenerAdapter;
import android.animation.ValueAnimator;
import android.os.AsyncTask;
import android.view.View;
import android.view.animation.AccelerateDecelerateInterpolator;

public class LanderAnimator extends AsyncTask<Integer,Integer,Integer> {

    private LunarView mLanderView;
    private ValueAnimator rocketfire;
    private int mX, mY;


    public LanderAnimator(LunarView lander){
        mLanderView = lander;
    }

    protected Integer doInBackground(Integer... integers){
        mX = integers[0];
        mY = integers[1];
        while(true){
            if(!mPaused&&!mFire){
                mX += 5;
                mY += 5;
                if (mX>=mLanderView.getWidth())
                    mX = 0;
                publishProgress(mX,mY);
            }
//            else if(mFire&&!mPaused){
//                mX += 5;
//                mY -= (int)(mLanderView.getWidth()/10);
//                if (mX>=mLanderView.getWidth())
//                    mX = 0;
//                publishProgress(mX,mY);
//                mFire = !mFire;
//            }
            try{
                Thread.sleep(100);
            }
            catch(Exception e){

            }
        }
    }
    @Override
    protected void onProgressUpdate(Integer... values){
        super.onProgressUpdate(values);

        mLanderView.setLanderPos(values[0],values[1]);
        mLanderView.invalidate();
    }

    private boolean mPaused = false;
    private boolean mFire = false;
    public void pauseAnimator(){
        mPaused = (!mPaused);
    }
    public void LanderFire() {
        mFire = (!mFire);
        rocketfire = ValueAnimator.ofInt(0, (int)(mLanderView.getHeight() / 10));
        rocketfire.setDuration(10);
        rocketfire.setInterpolator(new AccelerateDecelerateInterpolator());
        rocketfire.setRepeatCount(0);
        //rocketfire.setRepeatMode(ValueAnimator.RESTART);
        rocketfire.addUpdateListener(new ValueAnimator.AnimatorUpdateListener() {
            @Override
            public void onAnimationUpdate(ValueAnimator animation) {
                mY -= (int)animation.getAnimatedValue();
                publishProgress(mX, mY);
            }
        });
        rocketfire.start();
        mX += 5;
        //mY -= (int)(mLanderView.getWidth()/10);
        if (mX >= mLanderView.getWidth())
            mX = 0;
        publishProgress(mX, mY);
        mFire = !mFire;
//        rocketfire.addListener(new AnimatorListenerAdapter() {
//            public void onAnimationEnd(ValueAnimator animation) {
//                mFire = !mFire;
//            }
//        });
    }
}