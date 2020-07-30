//inicia el servidor
const app = require('./app');

app.listen(app.get('port'));
console.log('Servidor con puerto on en; ', app.get('port'));