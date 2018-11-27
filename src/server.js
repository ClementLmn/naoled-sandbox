const express = require('express');
const app = express();

const port = process.env.port || 3333;

app.post('/api', async (req, res) => {
  console.log(req.body);
  res.status(201).end();
})

app.listen(port, () => console.log(`Listening on port ${port}`))