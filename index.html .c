<!DOCTYPE html><html lang="en">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <title>Global Life Assistant MAX</title>  <style>
    :root{
      --bg:#0b1220;
      --card:#111b2e;
      --card2:#16233a;
      --text:#e5e7eb;
      --muted:#9ca3af;
      --accent:#22c55e;
      --danger:#ef4444;
      --blue:#3b82f6;
      --yellow:#f59e0b;
    }

    body.light{
      --bg:#f3f4f6;
      --card:#ffffff;
      --card2:#e5e7eb;
      --text:#111827;
      --muted:#4b5563;
    }

    *{box-sizing:border-box;}

    body {
      margin: 0;
      font-family: Arial, sans-serif;
      background: var(--bg);
      color: var(--text);
      transition:0.3s;
    }

    header {
      background: var(--card);
      padding: 16px;
      text-align: center;
      font-size: 20px;
      font-weight: bold;
      position: sticky;
      top: 0;
    }

    .topbar{
      display:flex;
      justify-content:space-between;
      align-items:center;
      padding:10px 15px;
      background:var(--card2);
      font-size:13px;
      color:var(--muted);
      flex-wrap:wrap;
      gap:10px;
    }

    .grid{
      display:grid;
      grid-template-columns:repeat(auto-fit,minmax(180px,1fr));
      gap:12px;
      padding:15px;
    }

    .card{
      background:var(--card);
      padding:14px;
      border-radius:14px;
      cursor:pointer;
      transition:0.2s;
      border:1px solid transparent;
    }

    .card:hover{
      transform:translateY(-4px);
      border:1px solid var(--accent);
    }

    .panel{
      display:none;
      position:fixed;
      top:8%;
      left:50%;
      transform:translateX(-50%);
      width:94%;
      max-width:540px;
      background:var(--card);
      padding:16px;
      border-radius:14px;
      z-index:10;
      box-shadow:0 20px 60px rgba(0,0,0,0.6);
    }

    input, select{
      width:100%;
      padding:10px;
      margin-top:8px;
      border-radius:10px;
      border:none;
      background:var(--card2);
      color:var(--text);
    }

    button{
      margin-top:8px;
      padding:10px;
      width:100%;
      border:none;
      border-radius:10px;
      background:var(--accent);
      color:white;
      font-weight:bold;
      cursor:pointer;
    }

    .smallBtn{
      width:auto;
      padding:6px 10px;
      font-size:12px;
      background:var(--blue);
      margin:2px;
    }

    ul{list-style:none;padding:0;}

    li{
      background:var(--card2);
      margin-top:6px;
      padding:10px;
      border-radius:8px;
      display:flex;
      justify-content:space-between;
    }

    .delete{color:var(--danger);cursor:pointer;}

    .statsBox{
      display:grid;
      grid-template-columns:repeat(2,1fr);
      gap:10px;
      padding:10px 15px;
    }

    .stat{
      background:var(--card);
      padding:12px;
      border-radius:12px;
      text-align:center;
    }

    canvas{
      width:100%;
      background:var(--card2);
      border-radius:10px;
      margin-top:10px;
    }
  </style></head>
<body><header>🌍 Global Life Assistant MAX</header><div class="topbar">
  <div id="xp">⭐ XP: 0</div>
  <div>
    <button class="smallBtn" onclick="toggleTheme()">🌓</button>
    <button class="smallBtn" onclick="exportData()">⬇ Export</button>
    <button class="smallBtn" onclick="importData()">⬆ Import</button>
  </div>
</div><div class="statsBox">
  <div class="stat">📅 Tasks: <span id="tCount">0</span></div>
  <div class="stat">💸 Money: <span id="mCount">0</span></div>
  <div class="stat">🧠 Habits: <span id="hCount">0</span></div>
  <div class="stat">🔥 Streak: <span id="streak">0</span></div>
</div><div class="grid">
  <div class="card" onclick="openPanel('tasks')">📅 Tasks</div>
  <div class="card" onclick="openPanel('money')">💸 Expenses</div>
  <div class="card" onclick="openPanel('habits')">🧠 Habits</div>
  <div class="card" onclick="openPanel('focus')">⏱ Focus</div>
</div><div id="tasks" class="panel">
  <h3>Tasks</h3>
  <input id="taskInput" placeholder="New task" />
  <button onclick="addTask()">Add</button>
  <input id="search" placeholder="Search tasks" oninput="render()" />
  <ul id="taskList"></ul>
</div><div id="money" class="panel">
  <h3>Expenses</h3>
  <input id="moneyInput" placeholder="Expense" type="number" />
  <button onclick="addExpense()">Add</button>
  <ul id="moneyList"></ul>
</div><div id="habits" class="panel">
  <h3>Habits</h3>
  <input id="habitInput" placeholder="New habit" />
  <button onclick="addHabit()">Add</button>
  <ul id="habitList"></ul>
  <canvas id="chart" height="120"></canvas>
</div><div id="focus" class="panel">
  <h3>Focus Timer</h3>
  <h2 id="timer">25:00</h2>
  <button onclick="startTimer()">Start</button>
  <button onclick="resetTimer()">Reset</button>
</div><script>

let time=1500,interval,xp=0;

function openPanel(id){document.getElementById(id).style.display='block';}
function closePanel(id){document.getElementById(id).style.display='none';}

function save(k,v){localStorage.setItem(k,JSON.stringify(v));}
function load(k){return JSON.parse(localStorage.getItem(k)||"[]");}

function addXP(v){xp+=v;document.getElementById('xp').innerText="⭐ XP: "+xp;}

function addTask(){let v=taskInput.value;if(v){let d=load("tasks");d.push(v);save("tasks",d);taskInput.value="";addXP(5);render();}}
function addExpense(){let v=moneyInput.value;if(v){let d=load("money");d.push(v);save("money",d);moneyInput.value="";addXP(2);render();}}
function addHabit(){let v=habitInput.value;if(v){let d=load("habits");d.push({name:v,done:false});save("habits",d);habitInput.value="";addXP(10);render();}}

function render(){
  let tasks=load("tasks");
  let q=document.getElementById("search")?.value||"";

  taskList.innerHTML=tasks.filter(t=>t.includes(q)).map((t,i)=>
    `<li>${t}<span class='delete' onclick='del("tasks",${i})'>x</span></li>`
  ).join("");

  moneyList.innerHTML=load("money").map((m,i)=>
    `<li>${m}$ <span class='delete' onclick='del("money",${i})'>x</span></li>`
  ).join("");

  let h=load("habits");
  habitList.innerHTML=h.map((x,i)=>
    `<li>${x.name}<span onclick='toggleHabit(${i})'>✔</span></li>`
  ).join("");

  document.getElementById('tCount').innerText=tasks.length;
  document.getElementById('mCount').innerText=load("money").length;
  document.getElementById('hCount').innerText=h.length;

  drawChart(h);
}

function del(k,i){let d=load(k);d.splice(i,1);save(k,d);render();}

function toggleHabit(i){let h=load("habits");h[i].done=!h[i].done;save("habits",h);addXP(8);render();}

function startTimer(){interval=setInterval(()=>{if(time>0){time--;update();}},1000)}
function resetTimer(){time=1500;update();clearInterval(interval)}
function update(){let m=Math.floor(time/60),s=time%60;timer.innerText=m+":"+(s<10?"0":"")+s;}

function toggleTheme(){document.body.classList.toggle("light")}

function exportData(){
  let data={tasks:load("tasks"),money:load("money"),habits:load("habits")};
  navigator.clipboard.writeText(JSON.stringify(data));
  alert("Data copied!");
}

function importData(){
  let d=prompt("Paste JSON");
  if(d){let obj=JSON.parse(d);for(let k in obj){save(k,obj[k]);}render();}
}

function drawChart(h){
  let c=document.getElementById("chart");
  if(!c)return;
  let ctx=c.getContext("2d");
  ctx.clearRect(0,0,400,200);
  let done=h.filter(x=>x.done).length;
  ctx.fillStyle="#22c55e";
  ctx.fillRect(20,20,done*20,30);
  ctx.fillStyle="#ef4444";
  ctx.fillRect(20,70,(h.length-done)*20,30);
}

render();update();

</script></body>
</html>