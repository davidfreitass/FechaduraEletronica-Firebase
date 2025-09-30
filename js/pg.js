const firebaseConfig = {
  apiKey: "AIzaSyBr_wtdWgDarvUqpdGgRA-6f33gSqlOg8M",
  authDomain: "fechaduraeleservo.firebaseapp.com",
  databaseURL: "https://fechaduraeleservo-default-rtdb.firebaseio.com",
  projectId: "fechaduraeleservo",
  storageBucket: "fechaduraeleservo.firebasestorage.app",
  messagingSenderId: "351327706537",
  appId: "1:351327706537:web:0c162f471efa22439378b8"
};
firebase.initializeApp(firebaseConfig);
const db = firebase.database();

const form = document.getElementById("form");
const nameEl = document.getElementById("name");
const deviceEl = document.getElementById("device");
const passEl = document.getElementById("pass");
const status = document.getElementById("status");
const tbody = document.getElementById("list");

form.addEventListener("submit", async (e) => {
  e.preventDefault();
  const name = nameEl.value.trim();
  const device = deviceEl.value.trim();
  const pass = passEl.value.trim();

  if (!name || !device || !pass) {
    status.textContent = "Preencha todos os campos!";
    return;
  }

  await db.ref("devices/" + device).set({ user: name, password: pass });
  status.textContent = "Salvo!";
  form.reset();
  load();
});

document.getElementById("clear").addEventListener("click", () => {
  form.reset();
  status.textContent = "";
});

function load() {
  db.ref("devices").once("value").then(snapshot => {
    const data = snapshot.val() || {};
    tbody.innerHTML = "";
    Object.keys(data).forEach(deviceId => {
      const tr = document.createElement("tr");
      tr.innerHTML = `
        <td>${deviceId}</td>
        <td>${data[deviceId].user}</td>
        <td>${data[deviceId].password}</td>
        <td><button onclick="del('${deviceId}')">Excluir</button></td>
      `;
      tbody.appendChild(tr);
    });
  });
}

function del(id) {
  db.ref("devices/" + id).remove().then(load);
}

load();
