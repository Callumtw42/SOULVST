
const express = require('express');
const bodyParser = require("body-parser");
const path = require('path');
const app = express();

app.use(bodyParser.json())
app.use(bodyParser.urlencoded({ extended: true }))
app.use(express.static(path.join(__dirname)))

app.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "graph.html"))
})

//listen
let server = app.listen('8000', (err) => {
    if (err) throw err;
    else console.log(`Server started on port 8000`);
});