#pragma once

/*
------------------------------------------------------------
GlassGarden — Web UI CSS Template

فایل CSS داشبورد Web UI
به صورت جداگانه برای خوانایی بهتر کد

Version : 1.1.0
------------------------------------------------------------
*/

const char WEBUI_CSS[] PROGMEM = R"cssraw(
  *{box-sizing:border-box;margin:0;padding:0}
  body{font-family:'Segoe UI',Tahoma,sans-serif;background:#0f172a;color:#e2e8f0;line-height:1.6;padding:16px}
  .container{max-width:800px;margin:0 auto}
  
  header {
    background-color: #e6ded1;
    border-radius: 20px;
    padding: 20px 10px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    position: relative;
    overflow: hidden;
    min-height: 110px;
  }

  header::before {
    content: "";
    position: absolute;
    left: 30px;
    top: 50%;
    transform: translateY(-50%);
    width: 120px;
    height: 120px;
    background-image: var(--img-logo);
    background-size: contain;
    background-repeat: no-repeat;
    background-position: center;
  }

  header::after {
    content: "";
    position: absolute;
    left: 50%;
    top: 50%;
    transform: translate(-50%, -50%);
    width: 200px;
    height: 120px;
    background-image: var(--img-nature);
    background-size: contain;
    background-repeat: no-repeat;
    background-position: center;
  }

  .status {
    position: relative;
    z-index: 1;
    display: flex;
    align-items: center;
    gap: 8px;
    background: rgba(255,255,255,0.3);
    padding: 8px 20px;
    border-radius: 20px;
    font-size: .7rem;
    color:#64748b;
    bottom:-30px;
  }
  
  .status-dot{width:10px;height:10px;border-radius:50%;background:#ef4444}
  .status-dot.online{background:#22c55e;box-shadow:0 0 8px #22c55e}
  
  header h1{color:#4a6741;font-size:1.6rem;position:relative;z-index:1}
  
  .grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(140px,1fr));gap:12px;margin-bottom:20px}
  .card{background:#1e293b;border-radius:12px;padding:16px;text-align:center;border:1px solid #334155;transition:.2s}
  .card:hover{border-color:#4ade80}
  .card-icon{font-size:2rem;margin-bottom:6px}
  .card-value{font-size:1.6rem;font-weight:700;color:#4ade80}
  .card-label{font-size:.85rem;color:#94a3b8;margin-top:4px}
  
  .controls{display:grid;grid-template-columns:repeat(auto-fit,minmax(150px,1fr));gap:12px;margin-bottom:20px}
  .ctrl-card{background:#1e293b;border-radius:12px;padding:16px;border:1px solid #334155}
  .ctrl-card h3{font-size:1rem;margin-bottom:12px;color:#cbd5e1;display:flex;align-items:center;gap:8px}
  .switch{position:relative;display:inline-block;width:56px;height:30px}
  .switch input{opacity:0;width:0;height:0}
  .slider{position:absolute;cursor:pointer;top:0;left:0;right:0;bottom:0;background:#475569;border-radius:30px;transition:.3s}
  .slider:before{position:absolute;content:"";height:22px;width:22px;left:4px;bottom:4px;background:#fff;border-radius:50%;transition:.3s}
  input:checked + .slider{background:#4ade80}
  input:checked + .slider:before{transform:translateX(26px)}
  .ctrl-row{display:flex;justify-content:space-between;align-items:center;margin-bottom:12px}
  .ctrl-state{font-size:.8rem;color:#94a3b8}
  
  .mode-bar{background:#1e293b;border-radius:12px;padding:16px;margin-bottom:20px;display:flex;justify-content:space-between;align-items:center;border:1px solid #334155}
  .mode-btn{background:#334155;border:none;color:#e2e8f0;padding:10px 24px;border-radius:8px;cursor:pointer;font-size:.95rem;transition:.2s}
  .mode-btn.active{background:#4ade80;color:#0f172a;font-weight:700}
  
  .logs{background:#1e293b;border-radius:12px;padding:16px;border:1px solid #334155;max-height:200px;overflow-y:auto}
  .logs h3{margin-bottom:10px;color:#cbd5e1;font-size:1rem}
  .log-entry{font-size:.8rem;padding:4px 0;border-bottom:1px solid #334155;color:#94a3b8}
  .log-entry:last-child{border:none}
  .log-time{color:#64748b}
  
  .footer{text-align:center;margin-top:24px;font-size:.75rem;color:#64748b}
  
  @media(max-width:480px){
    header h1{font-size:1.2rem}
    .card-value{font-size:1.3rem}
    header::before, header::after{width:160px;height:80px}
    header::before{left:-30px}
    header::after{left:55%;}
    .status{padding:8px 12px;right:-10px;}
  }
)cssraw";