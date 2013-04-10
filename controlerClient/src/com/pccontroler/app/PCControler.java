package com.pccontroler.app;

import java.io.IOException;
import java.util.HashSet;

import android.app.Application;
import android.content.Context;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.Vibrator;
import android.preference.PreferenceManager;
import android.util.Log;
import android.widget.Toast;

import com.pccontroler.R;
import com.pccontroler.action.AuthentificationAction;
import com.pccontroler.action.AuthentificationResponseAction;
import com.pccontroler.action.PCControlerAction;
import com.pccontroler.activity.connection.ConnectionListActivity;
import com.pccontroler.connection.Connection;
import com.pccontroler.connection.ConnectionList;
import com.pccontroler.protocol.PCControlerActionReceiver;
import com.pccontroler.protocol.PCControlerConnection;
import com.pccontroler.tcp.PCControlerConnectionUDP;

public class PCControler extends Application implements Runnable
{
	private static final long CONNECTION_CLOSE_DELAY = 3000;
	
	private SharedPreferences preferences;
	private Vibrator vibrator;
	
	private PCControlerConnection[] connection;
	
	private HashSet<PCControlerActionReceiver> actionReceivers;
	
	private Handler handler;
	
	private CloseConnectionScheduler closeConnectionScheduler;
	
	private ConnectionList connections;
	
	private boolean requestEnableBluetooth;
	
	private boolean isBroadcast;
	
	private PCControlerConnectionUDP connectUdp;
	
	private ConnectionListActivity broadcastAction;
	
	public void onCreate()
	{
		super.onCreate();
		
		this.preferences = PreferenceManager.getDefaultSharedPreferences(this);
		PreferenceManager.setDefaultValues(this, R.xml.settings, true);
		
		this.vibrator = (Vibrator) this.getSystemService(Context.VIBRATOR_SERVICE);
		
		this.actionReceivers = new HashSet<PCControlerActionReceiver>();
		
		this.handler = new Handler();
		
		this.connection = new PCControlerConnection[1];
		
		this.closeConnectionScheduler = new CloseConnectionScheduler();
		
		this.connections = new ConnectionList(this.preferences);
		
		this.requestEnableBluetooth = true;
		
		this.isBroadcast = false;
	}
	
	public SharedPreferences getPreferences()
	{
		return this.preferences;
	}
	
	public ConnectionList getConnections()
	{
		return this.connections;
	}
	
	public void vibrate(long l)
	{
		if (this.preferences.getBoolean("feedback_vibration", true))
		{
			this.vibrator.vibrate(l);
		}
	}
	
	public boolean requestEnableBluetooth()
	{
		boolean b = this.requestEnableBluetooth;
		
		this.requestEnableBluetooth = false;
		
		return b;
	}
	
	public synchronized void run()
	{
		// this.showInternalToast("connect");
		// if (this.isBroadcast)
		// {
		// this.runBroadcast();
		// return;
		// }
		
		Connection co = this.connections.getUsed();
		
		if (co != null)
		{
			PCControlerConnection c = null;
			
			try
			{
				c = co.connect(this);
				
				synchronized (this.connection)
				{
					this.connection[0] = c;
				}
				
				try
				{
					this.showInternalToast(R.string.text_connection_established);
					
					String password = co.getPassword();
					this.sendAction(new AuthentificationAction(password));
					
					while (true)
					{
						PCControlerAction action = c.receiveAction();
						
						this.receiveAction(action);
					}
				}
				finally
				{
					synchronized (this.connection)
					{
						this.connection[0] = null;
					}
					
					c.close();
				}
			}
			catch (IOException e)
			{
				this.debug(e);
				
				if (c == null)
				{
					this.showInternalToast(R.string.text_connection_refused);
				}
				else
				{
					this.showInternalToast(R.string.text_connection_closed);
				}
			}
			catch (IllegalArgumentException e)
			{
				this.debug(e);
				
				this.showInternalToast(R.string.text_illegal_connection_parameter);
			}
		}
		else
		{
			this.showInternalToast(R.string.text_no_connection_selected);
		}
	}
	
	// public void runBroadcast()
	// {
	// try
	// {
	// connectUdp = new
	// PCControlerConnectionUDP(PCControlerConnectionUDP.DEFAULT_PORT);
	//
	// connectUdp.sendAction();
	//
	// while (true)
	// {
	// BroadcastAction action = connectUdp.receiveAction();
	//
	// this.receiveAction(action);
	// }
	//
	// }
	// catch (SocketException e)
	// {
	// // TODO Auto-generated catch block
	// e.printStackTrace();
	// }
	// catch (IOException e)
	// {
	// // TODO Auto-generated catch block
	// e.printStackTrace();
	// }
	// }
	
	public void sendAction(PCControlerAction action)
	{
		synchronized (this.connection)
		{
			if (this.connection[0] != null)
			{
				try
				{
					this.connection[0].sendAction(action);
				}
				catch (IOException e)
				{
					this.debug(e);
				}
			}
		}
	}
	
	public void showInternalToast(int resId)
	{
		if (this.isInternalToast())
		{
			this.showToast(resId);
		}
	}
	
	public void showInternalToast(String message)
	{
		if (this.isInternalToast())
		{
			this.showToast(message);
		}
	}
	
	public boolean isInternalToast()
	{
		synchronized (this.actionReceivers)
		{
			return !this.actionReceivers.isEmpty();
		}
	}
	
	public void showToast(int resId)
	{
		this.showToast(this.getResources().getString(resId));
	}
	
	public void showToast(final String message)
	{
		this.handler.post(new Runnable()
		{
			public void run()
			{
				Toast.makeText(PCControler.this, message, Toast.LENGTH_SHORT).show();
			}
		});
	}
	
	private void receiveAction(PCControlerAction action)
	{
		synchronized (this.actionReceivers)
		{
			for (PCControlerActionReceiver actionReceiver : this.actionReceivers)
			{
				actionReceiver.receiveAction(action);
			}
		}
		
		if (action instanceof AuthentificationResponseAction)
		{
			this.receiveAuthentificationResponseAction((AuthentificationResponseAction) action);
		}
	}
	
	private void receiveAuthentificationResponseAction(AuthentificationResponseAction action)
	{
		if (action.authentificated)
		{
			this.showInternalToast(R.string.text_authentificated);
		}
		else
		{
			this.showInternalToast(R.string.text_not_authentificated);
		}
	}
	
	public void registerActionReceiver(PCControlerActionReceiver actionReceiver)
	{
		synchronized (this.actionReceivers)
		{
			this.actionReceivers.add(actionReceiver);
			
			if (this.actionReceivers.size() > 0)
			{
				synchronized (this.connection)
				{
					if (this.connection[0] == null)
					{
						(new Thread(this)).start();
					}
				}
			}
		}
	}
	
	// public void registerBroadcastReceiver(ConnectionListActivity
	// broadcastReceiver)
	// {
	// this.showInternalToast("connect");
	//
	// this.isBroadcast = true;
	//
	// this.broadcastAction = broadcastReceiver;
	//
	// (new Thread(this)).start();
	// // this.registerActionReceiver(broadcastReceiver);
	//
	// }
	//
	// public void unregisterBroadcastReceiver(ConnectionListActivity
	// actionReceiver)
	// {
	//
	// this.isBroadcast = false;
	//
	// this.connectUdp.close();
	//
	// this.broadcastAction = null;
	//
	// // this.unregisterActionReceiver(actionReceiver);
	//
	// }
	
	public void unregisterActionReceiver(PCControlerActionReceiver actionReceiver)
	{
		synchronized (this.actionReceivers)
		{
			
			this.actionReceivers.remove(actionReceiver);
			
			if (this.actionReceivers.size() == 0)
			{
				this.closeConnectionScheduler.schedule();
			}
		}
	}
	
	public void debug(Exception e)
	{
		if (this.preferences.getBoolean("debug_enabled", false))
		{
			Log.d(this.getResources().getString(R.string.app_name), null, e);
		}
	}
	
	private class CloseConnectionScheduler implements Runnable
	{
		private Thread currentThread;
		
		public synchronized void run()
		{
			try
			{
				this.wait(PCControler.CONNECTION_CLOSE_DELAY);
				
				synchronized (PCControler.this.actionReceivers)
				{
					if (PCControler.this.actionReceivers.size() == 0)
					{
						synchronized (PCControler.this.connection)
						{
							if (PCControler.this.connection[0] != null)
							{
								PCControler.this.connection[0].close();
								
								PCControler.this.connection[0] = null;
							}
						}
					}
				}
				
				this.currentThread = null;
			}
			catch (InterruptedException e)
			{
				PCControler.this.debug(e);
			}
			catch (IOException e)
			{
				PCControler.this.debug(e);
			}
		}
		
		public synchronized void schedule()
		{
			if (this.currentThread != null)
			{
				this.currentThread.interrupt();
			}
			
			this.currentThread = new Thread(this);
			
			this.currentThread.start();
		}
	}
}
