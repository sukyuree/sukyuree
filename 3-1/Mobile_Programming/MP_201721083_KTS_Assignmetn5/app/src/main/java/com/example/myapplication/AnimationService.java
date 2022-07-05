package com.example.myapplication;

import android.app.IntentService;
import android.content.Intent;
import android.os.Messenger;
import android.os.Message;
public class AnimationService extends IntentService {

    public AnimationService() {
        super("AnimationService");
    }

    @Override
    protected void onHandleIntent(Intent intent) {
        Messenger valMessenger = (Messenger)intent.getExtras().get("ValueMessenger");
        float startAngle = intent.getFloatExtra("StartValue",0);
        float endAngle = intent.getFloatExtra("EndValue",0);
        float incAngle = intent.getFloatExtra("IncValue",1);
        float stepTime = intent.getFloatExtra("UpdateTime",10);
        while(startAngle<endAngle){
            Message msg = Message.obtain();
            msg.obj = new Float(startAngle);
            try{
                valMessenger.send(msg);
                Thread.sleep((long)stepTime);
            }catch(android.os.RemoteException ex){

            }catch(InterruptedException ex){

            }
            if(MainActivity.is_animate == true) {
                startAngle += incAngle;
            }
        }

    }

    /**
     * Handle action Foo in the provided background thread with the provided
     * parameters
     *
     */
    private void handleActionFoo(String param1, String param2) {
        // TODO: Handle action Foo
        throw new UnsupportedOperationException("Not yet implemented");
    }

    /**
     * Handle action Baz in the provided background thread with the provided
     * parameters.
     */
    private void handleActionBaz(String param1, String param2) {
        // TODO: Handle action Baz
        throw new UnsupportedOperationException("Not yet implemented");
    }
}