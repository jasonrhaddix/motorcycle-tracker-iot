'use strict';
console.log('Loading function...');

let AWS = require("aws-sdk");
let DOC = require('dynamodb-doc');
let dynamoDB = new DOC.DynamoDB();

AWS.config.update({region: "us-west-2"});
 
exports.handler = (event, context, callback) => {
    
    console.log(event.payload.replace(/&quot;/g,'"'));
    event.payload = JSON.parse(event.payload.replace(/&quot;/g,'"'));
    
    var payload = event.payload[0];
    
    var params = {
        TableName: "gps_pos",
        Item: {
                timestamp: Number(payload.t),
                latitude: Number(payload.l),
                longitude: Number(payload.L)
              }
    };
    
    dynamoDB.putItem(params, function(err, data) {
        if (err) {
            context.done(err);
        }
        else {
            console.log('great success: '+JSON.stringify(data, null, '  '));
            // context.done('K THX BY');
        }
    });

};