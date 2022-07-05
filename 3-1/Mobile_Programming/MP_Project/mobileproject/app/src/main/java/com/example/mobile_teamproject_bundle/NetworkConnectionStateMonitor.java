package com.example.mobile_teamproject_bundle;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.net.ConnectivityManager;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.os.Build;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;

@RequiresApi(api = Build.VERSION_CODES.LOLLIPOP)
public class NetworkConnectionStateMonitor extends ConnectivityManager.NetworkCallback {

    private Context mContext;
    private NetworkRequest networkRequest;
    private ConnectivityManager mConnectivityManager;
    private boolean isNetworkConnected;

    public NetworkConnectionStateMonitor(Context context) {
        this.mContext = context;
        networkRequest = new NetworkRequest.Builder()
                .addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR)
                .addTransportType(NetworkCapabilities.TRANSPORT_WIFI)
                .build();
        mConnectivityManager = (ConnectivityManager) mContext.getSystemService(Context.CONNECTIVITY_SERVICE);
    }

    public void register() {
        mConnectivityManager.registerNetworkCallback(networkRequest, this);
    }

    public void unRegister() {
        mConnectivityManager.unregisterNetworkCallback(this);
    }

    @Override
    public void onAvailable(@NonNull Network network) {
        super.onAvailable(network);
        isNetworkConnected = true;
    }

    @Override
    public void onLost(@NonNull Network network) {
        super.onLost(network);
        isNetworkConnected = false;

        final AlertDialog alertDialog = new AlertDialog.Builder(mContext)
                .setCancelable(false)
                .setMessage(R.string.dialog_network_alert_message)
                .setPositiveButton(R.string.dialog_positive_button, null)
                .create();

        alertDialog.setOnShowListener(new DialogInterface.OnShowListener() {
            @Override
            public void onShow(DialogInterface dialog) {
                Button positiveButton = alertDialog.getButton(AlertDialog.BUTTON_POSITIVE);
                positiveButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        if (isNetworkConnected) {
                            alertDialog.dismiss();
                            Toast.makeText(mContext, R.string.network_connected_message, Toast.LENGTH_SHORT).show();
                        } else {
                            Toast.makeText(mContext, R.string.network_unconnected_message, Toast.LENGTH_SHORT).show();
                        }
                    }
                });
            }
        });

        alertDialog.show();
    }

    //TODO: check it again whether no problem
    public boolean isConnected() {
        return mConnectivityManager.getActiveNetwork() != null;
    }

}
