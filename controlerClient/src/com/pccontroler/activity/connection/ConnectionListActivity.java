package com.pccontroler.activity.connection;

import java.io.IOException;
import java.net.SocketException;
import java.util.ArrayList;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;

import com.pccontroler.R;
import com.pccontroler.action.BroadcastAction;
import com.pccontroler.activity.ControlActivity;
import com.pccontroler.app.PCControler;
import com.pccontroler.connection.Connection;
import com.pccontroler.connection.ConnectionList;
import com.pccontroler.connection.ConnectionWifi;
import com.pccontroler.tcp.PCControlerConnectionUDP;

public class ConnectionListActivity extends Activity implements OnItemClickListener, OnItemLongClickListener, OnClickListener, Runnable
{
	private static final int NEW_MENU_ITEM_ID = 0;
	
	private PCControler application;
	
	private ConnectionList connections;
	
	private ConnectionListAdapter adapter;
	
	private AlertDialog alertDialogNew;
	
	private AlertDialog alertDialogItemLongClick;
	
	private int itemLongClickPosition;
	
	private PCControlerConnectionUDP connectUdp;
	
	private Button buttonRefresh;
	
	private Button buttonNew;
	
	private ListView connectionsView;
	
	private Thread thread = null;
	
	private Handler messageHandler;
	
	private Button buttonSure;
	
	private Button buttonCancel;
	
	private EditText editName;
	
	private EditText editHost;
	
	private int newConnectionPosition;
	
	private ArrayList<Connection> userConnections;
	
	private AlertDialog.Builder builder;
	
	private boolean newConnection = true;
	
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		this.application = (PCControler) this.getApplication();
		
		this.connections = this.application.getConnections();
		
		this.userConnections = new ArrayList<Connection>();
		
		this.setContentView(R.layout.connectionlist);
		
		this.buttonRefresh = (Button) findViewById(R.id.refresh);
		
		this.buttonNew = (Button) findViewById(R.id.newconnect);
		
		this.connectionsView = (ListView) findViewById(R.id.connectionsView);
		
		this.connectionsView.setDivider(null);
		
		this.adapter = new ConnectionListAdapter(this, this.connections);
		
		this.connectionsView.setAdapter(this.adapter);
		
		this.connectionsView.setOnItemClickListener(this);
		
		this.connectionsView.setOnItemLongClickListener(this);
		
		this.buttonRefresh.setOnClickListener(new android.view.View.OnClickListener()
		{
			
			@Override
			public void onClick(View v)
			{
				refreshList();
			}
		});
		
		this.buttonNew.setOnClickListener(new android.view.View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				addConnection(0);
			}
		});
		
		Looper looper = Looper.myLooper();
		
		messageHandler = new MessageHandler(looper);
		
		this.init();
		
		this.refreshList();
		
	}
	
	public void refreshList()
	{
		this.closeThread();
		
		this.connections.clear();
		
		for (int i = 0; i < this.userConnections.size(); i++)
		{
			this.connections.append(this.userConnections.get(i));
		}
		
		this.refresh();
		
		this.thread = new Thread(this);
		
		this.thread.start();
	}
	
	public void closeThread()
	{
		if (this.thread != null)
		{
			this.thread.interrupt();
			this.connectUdp.close();
			this.thread = null;
		}
	}
	
	protected void onResume()
	{
		super.onResume();
		
		this.refresh();
		
		// this.broadcastSearch();
		
		// if (this.connections.getCount() == 0)
		// {
		// this.application.showToast(R.string.text_no_connection);
		// }
	}
	
	protected void onPause()
	{
		super.onPause();
		
		this.closeThread();
		
		// this.application.unregisterBroadcastReceiver(this);
		// this.connections.save();
	}
	
	// public void broadcastSearch()
	// {
	//
	// try
	// {
	// this.connectUdp.sendAction();
	// BroadcastAction action = this.connectUdp.receiveAction();
	// Toast.makeText(this.application, action.host, Toast.LENGTH_SHORT).show();
	// this.connectUdp.close();
	// }
	// catch (IOException e)
	// {
	// // TODO Auto-generated catch block
	// e.printStackTrace();
	// }
	// // Toast.makeText(this.application, "send",
	// // Toast.LENGTH_SHORT).show();
	//
	// // this.connectUdp.close();
	// // Toast.makeText(this.application, "receive",
	// // Toast.LENGTH_SHORT).show();
	// // Toast.makeText(this.application, , Toast.LENGTH_SHORT).show();
	//
	// }
	
	// public boolean onCreateOptionsMenu(Menu menu)
	// {
	// menu.add(Menu.NONE, NEW_MENU_ITEM_ID, Menu.NONE, R.string.text_new);
	//
	// return super.onCreateOptionsMenu(menu);
	// }
	
	// public boolean onOptionsItemSelected(MenuItem item)
	// {
	// switch (item.getItemId())
	// {
	// case NEW_MENU_ITEM_ID:
	// // this.alertDialogNew.show();
	// this.addConnection(0);
	// break;
	// }
	//
	// return true;
	// }
	
	public void onItemClick(AdapterView<?> parent, View view, int position, long id)
	{
		this.useConnection(position);
		this.startActivityForResult(new Intent(this, ControlActivity.class), 0);
	}
	
	public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id)
	{
		this.itemLongClickPosition = position;
		
		this.alertDialogItemLongClick.show();
		
		return true;
	}
	
	public void onClick(DialogInterface dialog, int which)
	{
		if (dialog == this.alertDialogNew)
		{
			// this.addConnection(which);
		}
		else if (dialog == this.alertDialogItemLongClick)
		{
			this.menu(which);
		}
	}
	
	private void menu(int which)
	{
		Connection connection = this.connections.get(this.itemLongClickPosition);
		
		switch (which)
		{
			case 0:
				this.useConnection(this.itemLongClickPosition);
				this.startActivityForResult(new Intent(this, ControlActivity.class), 0);
				break;
			case 1:
				// connection.edit(this);
				this.alertDialogNew.setTitle(R.string.text_edit);
				this.newConnection = false;
				this.editConnection(connection);
				break;
			case 2:
				this.removeConnection();
				break;
		}
	}
	
	private void addConnection(int which)
	{
		if (which == Connection.BLUETOOTH /*
										 * &&
										 * !BluetoothChecker.isBluetoohAvailable
										 * ()
										 */)
		{
			this.application.showToast(R.string.text_bluetooth_not_available_version);
		}
		else
		{
			this.alertDialogNew.setTitle(R.string.text_new);
			this.newConnection = true;
			Connection connection = this.connections.add(which);
			this.editConnection(connection);
			// this.refresh();
			
			// connection.edit(this);
		}
	}
	
	private void editConnection(Connection connection)
	{
		ConnectionWifi connectionWifi = (ConnectionWifi) connection;
		
		this.alertDialogNew.show();
		
		this.editName.setText(connection.getName());
		
		this.editHost.setText(connectionWifi.getHost());
		
		this.newConnectionPosition = this.connections.getPosition(connection);
	}
	
	private void useConnection(int position)
	{
		this.connections.use(position);
		this.refresh();
	}
	
	private void removeConnection()
	{
		Connection connection = this.connections.get(this.itemLongClickPosition);
		this.connections.remove(this.itemLongClickPosition);
		
		if (this.userConnections.contains(connection))
		{
			this.userConnections.remove(connection);
		}
		this.refresh();
	}
	
	private void refresh()
	{
		this.connections.sort();
		this.adapter.notifyDataSetChanged();
	}
	
	private void init()
	{
		// this.initAlertDialogNew();
		
		this.initAlertDialogItemLongClick();
		this.initAlertDialogItemNew();
	}
	
	private void initAlertDialogItemNew()
	{
		this.builder = new AlertDialog.Builder(this);
		
		LayoutInflater factory = LayoutInflater.from(this);
		
		View view = factory.inflate(R.layout.connectioncreatewifi, null);
		
		this.editName = (EditText) view.findViewById(R.id.name);
		
		this.editHost = (EditText) view.findViewById(R.id.host);
		
		this.buttonSure = (Button) view.findViewById(R.id.ok);
		
		this.buttonCancel = (Button) view.findViewById(R.id.cancel);
		
		this.builder.setTitle(R.string.text_new);
		this.builder.setView(view);
		
		this.alertDialogNew = this.builder.create();
		
		this.buttonSure.setOnClickListener(new android.view.View.OnClickListener()
		{
			
			@Override
			public void onClick(View v)
			{
				Connection connection = connections.get(newConnectionPosition);
				ConnectionWifi connectionWifi = (ConnectionWifi) connection;
				
				connectionWifi.setName(editName.getText().toString());
				connectionWifi.setHost(editHost.getText().toString());
				
				if (newConnection)
					userConnections.add(connection);
				
				refresh();
				alertDialogNew.dismiss();
			}
		});
		this.buttonCancel.setOnClickListener(new android.view.View.OnClickListener()
		{
			
			@Override
			public void onClick(View v)
			{
				if (newConnection)
					connections.remove(newConnectionPosition);
				
				alertDialogNew.dismiss();
			}
		});
		
	}
	
	// private void initAlertDialogNew()
	// {
	// String[] connectionTypeName = {
	// this.getResources().getString(R.string.text_wifi),
	// this.getResources().getString(R.string.text_bluetooth)
	// };
	//
	// AlertDialog.Builder builder = new AlertDialog.Builder(this);
	// builder.setTitle(R.string.text_connection_type);
	// builder.setItems(connectionTypeName, this);
	// this.alertDialogNew = builder.create();
	// }
	
	private void initAlertDialogItemLongClick()
	{
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setItems(R.array.connection_action_name, this);
		this.alertDialogItemLongClick = builder.create();
	}
	
	private class ConnectionListAdapter extends BaseAdapter
	{
		private ConnectionList connections;
		private LayoutInflater layoutInflater;
		
		private int connectionUsedPosition;
		
		public ConnectionListAdapter(Context context, ConnectionList connections)
		{
			this.connections = connections;
			
			this.layoutInflater = (LayoutInflater) context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			
			this.connectionUsedPosition = this.connections.getUsedPosition();
		}
		
		public void notifyDataSetChanged()
		{
			super.notifyDataSetChanged();
			
			this.connectionUsedPosition = this.connections.getUsedPosition();
		}
		
		public int getCount()
		{
			return this.connections.getCount();
		}
		
		public Connection getItem(int position)
		{
			return this.connections.get(position);
		}
		
		public long getItemId(int position)
		{
			return position;
		}
		
		// public View getTopView(View convertView, ViewGroup parent)
		// {
		// TopViewHolder holder;
		//
		// if (convertView == null)
		// {
		// holder = new TopViewHolder();
		//
		// convertView = this.layoutInflater.inflate(R.layout.topbuttons, null);
		//
		// holder.btnRefresh = (Button)
		// convertView.findViewById(R.id.buttonLeft);
		//
		// holder.btnNew = (Button) convertView.findViewById(R.id.buttonRight);
		//
		// convertView.setTag(holder);
		// }
		// else
		// {
		// holder = (TopViewHolder) convertView.getTag();
		// }
		//
		// holder.btnRefresh.setText("刷新");
		// holder.btnNew.setText("新建");
		// return convertView;
		// }
		
		public View getView(int position, View convertView, ViewGroup parent)
		{
			ConnectionViewHolder holder;
			
			if (convertView == null)
			{
				holder = new ConnectionViewHolder();
				
				convertView = this.layoutInflater.inflate(R.layout.connection, null);
				
				// holder.use = (RadioButton)
				// convertView.findViewById(R.id.use);
				holder.icon = (ImageView) convertView.findViewById(R.id.icon);
				holder.name = (TextView) convertView.findViewById(R.id.name);
				
				convertView.setTag(holder);
			}
			else
			{
				holder = (ConnectionViewHolder) convertView.getTag();
			}
			
			Connection connection = this.connections.get(position);
			
			// holder.use.setChecked(position == this.connectionUsedPosition);
			
			holder.icon.setImageResource(R.drawable.monitor);
			holder.name.setText(connection.getName());
			
			// if (connection instanceof ConnectionWifi)
			// {
			// ConnectionWifi co = (ConnectionWifi) connection;
			// holder.icon.setImageResource(R.drawable.wifi);
			// holder.name.setText(co.getHost());
			// }
			/*
			 * else if (connection instanceof ConnectionBluetooth) {
			 * holder.icon.setImageResource(R.drawable.bluetooth); }
			 */
			
			// holder.name.setText(connection.getName());
			
			return convertView;
		}
		
		private class ConnectionViewHolder
		{
			// public RadioButton use;
			public ImageView icon;
			public TextView name;
		}
		
	}
	
	public class MessageHandler extends Handler
	{
		public MessageHandler(Looper looper)
		{
			super(looper);
		}
		
		@Override
		public void handleMessage(Message msg)
		{
			BroadcastAction action = (BroadcastAction) msg.obj;
			if (action == null)
				
				return;
			
			Connection connection = connections.add(0);
			
			ConnectionWifi wifi = (ConnectionWifi) connection;
			
			connection.setName(action.host);
			connection.setPassword("aa12");
			wifi.setPort(action.port);
			wifi.setHost(action.address);
			
			refresh();
		}
	}
	
	public void addConnect(BroadcastAction action)
	{
		// if (action == null)
		// return;
		//
		// Connection connection = this.connections.add(0);
		//
		// ConnectionWifi wifi = (ConnectionWifi) connection;
		//
		// connection.setName(action.host);
		// connection.setPassword("aa12");
		// wifi.setPort(action.port);
		// wifi.setHost(action.address);
		//
		// this.refresh();
		Message message = Message.obtain();
		
		message.obj = action;
		
		messageHandler.sendMessage(message);
	}
	
	@Override
	public void run()
	{
		try
		{
			this.connectUdp = new PCControlerConnectionUDP(PCControlerConnectionUDP.DEFAULT_PORT);
			
			this.connectUdp.sendAction();
			
			while (!Thread.interrupted())
			{
				BroadcastAction action = this.connectUdp.receiveAction();
				this.addConnect(action);
				// this.application.showInternalToast(action.host);
			}
			
		}
		catch (SocketException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		// TODO Auto-generated method stub
		catch (IOException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
