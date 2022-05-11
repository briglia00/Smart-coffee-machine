package smartcm;


import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import smartcm.*;

public class smartcm extends JFrame {
    
    private static final long serialVersionUID = -6218820567019985015L;
    private SerialCommChannel channel;
    

    public smartcm(int size, SerialCommChannel scc) {
    	
    	Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
            public void run() {
                //clearAll();
            }
        }, "Shutdown-thread"));
    	
        this.setDefaultCloseOperation(DISPOSE_ON_CLOSE);
        this.channel = scc;
        this.setTitle("Smart Coffee Machine");
        this.setSize(180*size, 70*size);
        
        
        JPanel panel = new JPanel(new GridLayout(size-1,size));
        JLabel l1 = new JLabel("");
        JLabel l2 = new JLabel("");
        JLabel textstat = new JLabel("None");
        JLabel textgself = new JLabel("");
        
        JButton jgcof = new JButton("Get Available Coffee");
        JButton jgchoc = new JButton("Get Available Chocolate");
        JButton jgtea = new JButton("Get Available Tea");
        JButton jcof = new JButton("Refill Coffee");
        JButton jtea = new JButton("Refill Tea");
        JButton jchoc = new JButton("Refill Chocolate");
        JButton jstat = new JButton("Get Status");
        JButton jself = new JButton("Number of Self Tests Completed");
        
        JButton jrecover = new JButton("Recover Broken Machine");
        
        JLabel textgcof = new JLabel("");
        JLabel textgchoc = new JLabel("");
        JLabel textgtea = new JLabel("");
        JTextField textcof = new JTextField("0");
        JTextField texttea = new JTextField("0");
        JTextField textchoc = new JTextField("0");
        
        
        this.getContentPane().add(panel);
        this.setVisible(true);
        panel.add(jgcof); panel.add(textgcof);
        panel.add(jcof); panel.add(textcof);
        panel.add(jrecover);
        panel.add(jgtea); panel.add(textgtea);
        panel.add(jtea); panel.add(texttea);
        panel.add(l2);
        panel.add(jgchoc); panel.add(textgchoc);
        panel.add(jchoc); panel.add(textchoc);
        panel.add(l1);
        panel.add(jstat); panel.add(textstat);
        panel.add(jself); panel.add(textgself);
        
        
        ActionListener refCoffee = e -> {
        	try {
        		int temp = Integer.parseInt(textcof.getText());
        		if (temp < 0 || temp > 99) throw new NumberFormatException();
        		textcof.setBackground(Color.white);
        		channel.sendMsg("REFCOF-" + temp);
        	} catch (NumberFormatException e1) {
        		textcof.setBackground(Color.red);
        	}
        };
        
        ActionListener refTea = e -> {
        	try {
        		int temp = Integer.parseInt(texttea.getText());
        		if (temp < 0 || temp > 99) throw new NumberFormatException();
        		texttea.setBackground(Color.white);
        		channel.sendMsg("REFTEA-" + temp);
        	} catch (NumberFormatException e1) {
        		texttea.setBackground(Color.red);
        	}
        };
        
        ActionListener refChoc = e -> {
        	try {
        		int temp = Integer.parseInt(textchoc.getText());
        		if (temp < 0 || temp > 99) throw new NumberFormatException();
        		textchoc.setBackground(Color.white);
        		channel.sendMsg("REFCHO-" + temp);
        	} catch (NumberFormatException e1) {
        		textchoc.setBackground(Color.red);
        	}
        };
        
        ActionListener getStatus = e -> {
        	try {
        		channel.sendMsg("GETSTS");
        		String msg = channel.receiveMsg();
        		
        		if(msg == "0") {
        			textstat.setText("Inizializing");
        		} else if (msg.equals("1")) {
        			textstat.setText("Maintenance");
        		} else if (msg.equals("2")) {
        			textstat.setText("Ready");
        		} else if (msg.equals("3")) {
        			textstat.setText("Making Products");
        		} else if (msg.equals("4")) {
        			textstat.setText("Product Done");
        		} else if (msg.equals("5")) {
        			textstat.setText("Sleeping");
        		} else if (msg.equals("6")) {
        			textstat.setText("Testing");
        		} else if (msg.equals("7")) {
        			textstat.setText("Broken");
        		}
        	} catch (Exception e1) {
        		
        	}
        };
        
        ActionListener getCof = e -> {
        	try {
        		channel.sendMsg("GETCOF");
        		String msg = channel.receiveMsg();
        		textgcof.setText(msg);
        	} catch (Exception e1) {
        		
        	}
        };
        
        ActionListener getTea = e -> {
        	try {
        		channel.sendMsg("GETTEA");
        		String msg = channel.receiveMsg();
        		textgtea.setText(msg);
        	} catch (Exception e1) {
        		
        	}
        };
        
        ActionListener getChoc = e -> {
        	try {
        		channel.sendMsg("GETCHO");
        		String msg = channel.receiveMsg();
        		textgchoc.setText(msg);
        	} catch (Exception e1) {
        		
        	}
        };
        
        ActionListener getSelfTest = e -> {
        	try {
        		channel.sendMsg("GETSLF");
        		String msg = channel.receiveMsg();
        		textgself.setText(msg);
        	} catch (Exception e1) {
        		
        	}
        };
        
        ActionListener recoverMachine = e -> {
        	try {
        		channel.sendMsg("RECOVR");
        	} catch (Exception e1) {
        		
        	}
        };
        
        jself.addActionListener(getSelfTest);
        jcof.addActionListener(refCoffee);
        jtea.addActionListener(refTea);
        jchoc.addActionListener(refChoc);
        jgcof.addActionListener(getCof);
        jgtea.addActionListener(getTea);
        jgchoc.addActionListener(getChoc);
        jstat.addActionListener(getStatus);
        jrecover.addActionListener(recoverMachine);
    }
    
    public static void main(String[] args) throws Exception {
    	SerialCommChannel channel = new SerialCommChannel("/dev/ttyACM2",9600);
    	Thread.sleep(4000);
    	new smartcm(5, channel);
    }
}