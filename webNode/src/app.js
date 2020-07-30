//configuraciones del server
const express = require('express');
const morgan = require('morgan');
const exphbs = require('express-handlebars');
const path = require('path');

const app = express();
 
    //settings si uso npm i nodemon -D el servidor automaticamente ve los cambios
        app.set('port', process.env.PORT || 9000);
            //especificanis ruta de la carpeta
        app.set('views', path.join(__dirname,'views'));
        //configuramos motor de plantillas
        app.engine('.hbs', exphbs({
            //codigo wn común routes
            defaultLayout: 'main',
            extname: '.hbs'
        }));
        app.set('view engine','.hbs');
    //funciones intermedas middlewears
        app.use(morgan('dev'));
        //acepta datos de las rutas
        app.use(express.urlencoded({extended: false}));
    //routes
        app.use(require('./routes/index.js'));
        
    //static files
        //archivos publicos
        app.use(express.static(path.join(__dirname,'public')));

    module.exports = app;   