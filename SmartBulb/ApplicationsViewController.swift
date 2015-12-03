//
//  ApplicationsViewController.swift
//  SmartBulb
//
//  Created by Mac on 11/30/15.
//  Copyright © 2015 GTS. All rights reserved.
//

import UIKit

class ApplicationsViewController: UIViewController {
    
    let deligate = UIApplication.sharedApplication().delegate as! AppDelegate
    
    @IBOutlet weak var bulb: UIImageView!
    @IBOutlet weak var mySwitch: UISwitch!
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        
        let rx_thread = NSThread(target:self, selector:"start_rx", object:nil)
        rx_thread.start();
        
        // Do any additional setup after loading the view, typically from a nib.
    }
    
    func refresh(sender:AnyObject)
    {
        bulb.image = UIImage(named: ("on.png"))
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func start_rx(){
        while(true){
            if(view.window != nil){
                deligate.txrx!.start_rx();
            }
            NSThread .sleepForTimeInterval(0.5);
        }
    }
    
    @IBAction func change(sender: AnyObject) {
        
        if(mySwitch.on){
            deligate.txrx!.send("c");
            bulb.image = UIImage(named: ("on.png"))
        }else{
            deligate.txrx!.send("o");
            bulb.image = UIImage(named: ("off.png"))
        }
    }

}
