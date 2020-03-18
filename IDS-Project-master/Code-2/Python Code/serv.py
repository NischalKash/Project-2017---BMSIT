from flask import Flask,render_template
from flask import request
import json
import MySQLdb
import webbrowser

class mydata:

    def __init__(self): #initialize the class when launching app
        self.pino = None
        self.uid = None
        self.name = None
        self.authorization = 'Unauthorized'
        self.time = None
        self.fid = None

    def get_val(self, data, f): #get the values of data incoming for rfid phase
            self.pino = data['pino']
            self.uid = data['uid']
            self.time = data['time']
            if f == True: #if rfid tag valid
                self.name = data['name']
                self.rfid = " RFID Verified"
                self.fid = data['fid']
            else: #if rfid tag is invalid, name is Unknown and written to the log file automatically
                self.rfid = "Invalid"
                self.name = 'Unknown'
                self.write_data()

    def write_data(self): #wrting data to log file
        stg = self.pino + ' ' + self.uid + ' ' + self.name + ' ' + self.time + ' ' + self.rfid + ' '+ self.authorization + '\n'
        with open('authorize.log', 'a') as f:
            f.write(stg)

    def ret_data(self): #returning the latest events
        with open('authorize.log', 'r') as f:
            dat = f.read().split('\n')
            dat.reverse()
            d = []
            for line in dat:
                 if line != '':
                     d.append(line)
        return d

    def calc_auth(self):
        if self.rfid and self.face:
            self.authorization = True

#class ended
dt = mydata()
app = Flask(__name__)

@app.route('/', methods = ['POST'])
def determine_escalation(): #for rfid verification only
    jsondata = request.get_json()
    data = json.loads(jsondata)
    enc_tag = "".join("{:02x}".format(ord(c)) for c in data['uid']) #encoding the uid in a format readable by mysql
    db = MySQLdb.connect(host = 'localhost', user = 'root', passwd = '13sep95', db = 'project') #connect to database
    str = 'select name,face_id from rfid where uid = ' + enc_tag #the mysql query
    cur = db.cursor()
    num = cur.execute(str) #execute the query
    if num != 0: #checking if vaild details exist
        for row in cur.fetchall():
            data['name'] = row[0]
            data['fid'] = row[1]
        dt.get_val(data, True)
        r = {'result' : 'Authorized'}
    else:
        '''since rfid is invalid,
        the person is directly classified as an intruder
        and there will be no facial recognition phase '''
        dt.get_val(data, False)
        r = {'result' : 'Unauthorized'}
    db.close()

    return json.dumps(r)

@app.route('/facerec', methods = ['POST'])
def write_data():#for obtaining and processing the results of facial recognition
    jsdata = request.get_json()
    d = json.loads(jsdata)
    print(d['fid'])
    r = {'val' : 'Unauthorized'}
    if d['fid'] == dt.fid:
        dt.authorization = 'Authorized'
        r['val'] = 'Authorized'

    dt.write_data()

    return json.dumps(r)

@app.route('/check')
def check(): #displays the log details in a webpage for system admin
    rd = dt.ret_data()
    return render_template('alert.html', result = rd)

webbrowser.open_new_tab('http://localhost:5000/check')
if __name__ == '__main__':
    app.run(host = "0.0.0.0",port = 5000,debug=True)
