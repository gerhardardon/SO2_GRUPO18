import React, { useEffect, useState } from "react";
import axios from "axios";
import "chart.js/auto";
import { Pie } from "react-chartjs-2";
import "./dashboard.css";

const Dashboard = () => {
  const [data, setData] = useState([]);
  const [proc, setProc] = useState([]);

  // Función para obtener datos de la API
  const fetchData = async () => {
    try {
      const response = await axios.get(
        "http://localhost:5200/api/data/llamadas"
      );
      setData(response.data);
      //console.log('Datos:', data);  // Imprimir datos
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  };

  // Función para obtener procesos de la API
  const fetchProc = async () => {
    try {
      const response = await axios.get(
        "http://localhost:5200/api/data/procesos"
      );
      setProc(response.data);
      //console.log('Procesos:', proc);  // Imprimir datos obtenidos
    } catch (error) {
      console.error("Error fetching data:", error);
    }
  };

  useEffect(() => {
    // Llamar a fetchData al montar el componente
    fetchData();
    fetchProc();
    // Llamar a fetchData cada 5 segundos
    const intervalId = setInterval(() => {
      fetchData();
      fetchProc();
      //console.log('Datos:', data);  // Imprimir datos
    }, 5000);

    console.log("Datos:", data); // Imprimir datos
    console.log("Procesos:", proc); // Imprimir datos obtenidos

    // Limpiar el intervalo cuando el componente se desmonte
    return () => clearInterval(intervalId);
  }, []);

  const opciones = {
    maintainAspectRatio: false,
    responsive: true,
    // hacemos que la leyenda sea visible
    plugins: {
      tooltip: {
        titleFont: {
          size: 20,
        },
        bodyFont: {
          size: 20,
        },
      },
      legend: {
        labels: {
          font: {
            size: 18,
          },
        },
      },
    },
  };
  // Calcular la suma de los porcentajes de memoria
  const totalPorcentaje = proc.reduce(
    (acc, proceso) => acc + proceso.porcentaje_memoria * 100,
    0
  );

  // Calcular la parte restante para llegar al 100%
  const restantePorcentaje = 100 - totalPorcentaje;

  // Datos para la gráfica de pastel
  const pieData = {
    labels: [...proc.map((proceso) => proceso.nombre), "Restante"],
    datasets: [
      {
        data: [
          ...proc.map((proceso) => proceso.porcentaje_memoria * 100),
          restantePorcentaje,
        ],
        backgroundColor: [
          ...proc.map((_, index) => {
            const colors = [
              "#FF6384",
              "#36A2EB",
              "#FFCE56",
              "#4BC0C0",
              "#9966FF",
              "#FF9F40",
              "#FFCD56",
              "#742919",
              "#F54D29",
              "#FAB7A9",
            ]; // Colores para las partes
            return colors[index % colors.length];
          }),
          "#CCCCCC", // Color para la parte restante
        ],
        hoverBackgroundColor: [
          ...proc.map((_, index) => {
            const colors = [
              "#FF6384",
              "#36A2EB",
              "#FFCE56",
              "#4BC0C0",
              "#9966FF",
              "#FF9F40",
              "#FFCD56",
              "#742919",
              "#F54D29",
              "#FAB7A9",
            ]; // Colores para hover
            return colors[index % colors.length];
          }),
          "#AAAAAA", // Color hover para la parte restante
        ],
      },
    ],
  };

  return (
    <div>
      <div className="dashboard-title-container">
        <h1 className="dashboard-title">Dashboard</h1>
      </div>
      <div className="tablas">
        <div className="dashboard-table2-container">
        <table className="dashboard-table">
          <thead>
            <tr>
              <th colSpan="5">Llamadas</th> {/* Título para la primera tabla */}
            </tr>
            <tr>
              <th>PID</th>
              <th>Nombre</th>
              <th>Llamada</th>
              <th>Tamaño</th>
              <th>Fecha</th>
            </tr>
          </thead>
          <tbody>
            {data.map((solicitud) => (
              <tr key={solicitud.id}>
                <td>{solicitud.pid}</td>
                <td>{solicitud.nombre}</td>
                <td>{solicitud.llamada}</td>
                <td>{solicitud.tamano}</td>
                <td>{solicitud.fecha}</td>
              </tr>
            ))}
          </tbody>
        </table>
        </div>
        <div className="dashboard-table2-container">
          <table className="dashboard-table">
            
          <thead>
            <tr>
              <th colSpan="4">Procesos</th> {/* Título para la segunda tabla */}
            </tr>
            <tr>
              <th>pid</th>
              <th>Nombre</th>
              <th>Memoria</th>
              <th>Porcentaje</th>
            </tr>
          </thead>
          <tbody>
            {proc.map(proceso => (
              <tr key={proceso.pid}>
                <td>{proceso.pid}</td>
                <td>{proceso.nombre}</td>
                <td>{proceso.memoriamb}</td>
                <td>{proceso.porcentaje_memoria * 100}%</td>
              </tr>
            ))}
          </tbody>
          </table>
        </div>
      </div>
      <div className="dashboard-pie-container">
        <Pie data={pieData} options={opciones} style={{ height: "50vh" }} />
      </div>
    </div>
  );
};

export default Dashboard;
