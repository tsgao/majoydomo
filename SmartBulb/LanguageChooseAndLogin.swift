//
//  languageChooseAndLogin.swift
//  SmartBulb
//
//  Created by Frank Gao on 12/2/15.
//  Copyright © 2015 GTS. All rights reserved.
//
import UIKit
import AVFoundation

//class PikerController: UIViewController, UIPickerViewDelegate, UIPickerViewDataSource {

class LanguageChooseAndLogin: UIViewController {
    
     let deligate = UIApplication.sharedApplication().delegate as! AppDelegate
    
    //@IBOutlet weak var picker: UIPickerView!
    //@IBOutlet weak var myLabel: UILabel!
    @IBOutlet weak var userName: UITextField!
    @IBOutlet weak var password: UITextField!
    @IBOutlet weak var okButton: UIButton!
    //@IBOutlet weak var label: UILabel!
    
    //var pickLan : [String] = [String]()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        /*
        if ( NSUserDefaults.standardUserDefaults().stringForKey("usernameKey") == nil && NSUserDefaults.standardUserDefaults().stringForKey("passwordKey") == nil){
            NSUserDefaults.standardUserDefaults().setObject("", forKey: "usernameKey")
            NSUserDefaults.standardUserDefaults().setObject("", forKey: "passwordKey")
        } else if (  NSUserDefaults.standardUserDefaults().stringForKey("passwordKey") == nil){
            NSUserDefaults.standardUserDefaults().setObject("", forKey: "usernameKey")
            password.text = NSUserDefaults.standardUserDefaults().stringForKey("passwordKey")!
        } else if ( NSUserDefaults.standardUserDefaults().stringForKey("usernameKey") == nil){
            NSUserDefaults.standardUserDefaults().setObject("", forKey: "passwordKey")
            userName.text = NSUserDefaults.standardUserDefaults().stringForKey("usernameKey")!
        } else {
            userName.text = NSUserDefaults.standardUserDefaults().stringForKey("usernameKey")!
            password.text = NSUserDefaults.standardUserDefaults().stringForKey("passwordKey")!
        }*/
        
        //piker codes:
        /*
        self.picker.delegate = self
        self.picker.dataSource = self
        
        pickLan = ["العربية", "中文", "English"]
        */
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    @IBAction func firstTimeSet(sender: AnyObject) {
        
        if (userName.text != "" && password.text != ""){
            //NSUserDefaults.standardUserDefaults().setObject(myLabel.text, forKey: "language")
            
            NSUserDefaults.standardUserDefaults().setObject(userName.text, forKey: "userNameKey")
            NSUserDefaults.standardUserDefaults().setObject(password.text, forKey: "passwordKey")
            
            //isLogInSuccess(userName.text!, passco: password.text!)){
            //prounce("congradulations, your user name and password are correct. You are logged in now")
            
            if(deligate.txrx?.login(userName.text!,password: password.text!)==true){
                self.performSegueWithIdentifier("successChangeView", sender: nil)
            }
            
            //} else {
            //    prounce("Sorry, you user name or password is not correct. Please recheck your user name and password then try to login again")
            //}
        
        } else {
            prounce("Please type in your username and password and click check mark to login again")
        }
    }
    
    func prounce (words: String){
        let utt = AVSpeechUtterance(string: words)
        utt.voice = AVSpeechSynthesisVoice(language: "en-GB")
        utt.rate = 0.45
        let syn = AVSpeechSynthesizer()
        syn.speakUtterance(utt)
    }
    
    /*
    func numberOfComponentsInPickerView(pickerView: UIPickerView) -> Int {
        return 1
    }
    
    func pickerView(pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
        return pickLan.count
    }
    
    //MARK: Delegates
    func pickerView(pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
        return pickLan[row]
    }
    
    func pickerView(pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
        myLabel.text = pickLan[row]
    }
    */
    
}