//OBJETOS PARA EL ENRUTADOR
const { Router }= require('express');
const { route } = require('../app');
const router = Router();

//conexión firebase
const admin =require('firebase-admin');
const { firestore } = require('firebase-admin');
admin.initializeApp({
    credential: admin.credential.applicationDefault(),
    databaseURL: 'https://esp32firebase-34f7c.firebaseio.com/'
});//al configurar su conexión puede ingresar el comando npm run dev 

//objeto de conexion
const db = admin.database();

router.get('/', (req, res) => {
   
   res.render('index');
});
router.post('/consultar', (req, res) => {
    console.log(req.body.ciudad);
    const nuevo = req.body.ciudad
    db.ref('MiAmbiente/'+nuevo).once('value', (snapshop) => {
    const datos = snapshop.val();
    console.log(datos);
    res.render('./',{ambiente: datos});
    });
   
});
module.exports = router;

