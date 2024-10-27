
import React, { useState, useEffect, useRef } from 'react';
import DataCard from './DataCard';
import Graph from './Graph';
import { w3cwebsocket as W3CWebSocket } from "websocket";


import { addDoc, collection } from 'firebase/firestore';
import { db } from '../services/firebase';
import { NEEDS_TO_BE_STOPPED, NOT_STARTED, PDF_NAME_REGEX, PRINT, SAVE, STARTED, STOPPED } from "../constants.js"
import Sidebar from './Sidebar.jsx';
import Table from './Table.jsx';
import saveAsPDF from '../helper/saveaspdf.js';


const STATUS = [NOT_STARTED, STARTED, NEEDS_TO_BE_STOPPED, STOPPED]



let readings = []

const Dashboard = () => {
  const [is_saving_result, set_is_saving_result] = useState(false)
  const [show_modal, set_show_modal] = useState(false);
  const [btn_action, set_btn_action] = useState("")
  const [input_value, set_input_value] = useState("");
  const [status, setStatus] = useState(STATUS[0]);
  const websocket = useRef(null);
  const [pdf_name, set_pdf_name] = useState("")

  const [test_info, set_test_info] = useState([])
  const [dashboard_info, set_dashboard_info] = useState({})



  // console.log(readings)
  console.log({ dashboard_info })

  useEffect(() => {
    websocket.current = new W3CWebSocket('ws://isutmx.local/ws');
    websocket.current.onmessage = (message) => {
      const dataFromServer = JSON.parse(message.data);
      if (dataFromServer["type"] == "dashboard_page_info") {
        if (dataFromServer["sensor_rating_kg"] == "-1") {
          dataFromServer["sensor_rating_kg"] = "----"
        } else {
          dataFromServer["sensor_rating_kg"] = (dataFromServer["sensor_rating_kg"] * 9.81).toFixed(1)

        }
        if (dataFromServer["target_force"] == "-1") {
          dataFromServer["target_force"] = "----"
        }
        if (dataFromServer["target_extension"] == "-1") {
          dataFromServer["target_extension"] = "----"
        }

        set_dashboard_info({ ...dashboard_info, ...dataFromServer })
      } else if (dataFromServer["type"] == "update") {

        // setStatus(true)
        delete dataFromServer["type"];

        readings.push(dataFromServer)
        console.log({ readings })
        set_test_info(JSON.parse(JSON.stringify(readings)))

        // set_y_axis(dataFromServer["force_in_newton"])

      } else if (dataFromServer["type"] == "status") {

        let newStatusId = dataFromServer["status"]
        if ((status == STOPPED || status == NOT_STARTED) && STATUS[newStatusId] == STARTED) {

          readings = [];
          set_test_info(JSON.parse(JSON.stringify(readings)))
        }
        // if(status == STOPPED){

        // }

        setStatus(STATUS[newStatusId]);

      }

    };
    return () => websocket.current.close();
  }, [])

  // const pdfRef = React.createRef();
  // console.log({ test_info })

  // console.log({
  //   x_axis, y_axis
  // })
  return (
    <section className='pb-8'>
      <p className='font-faster font-normal text-4xl md:text-4xl text-primary text-center my-3 '>
        isutmx
      </p>
      <div className="  flex flex-col  justify-around gap-8 ">
        {
          show_modal ?
            <div className='bg-black opacity-90 w-screen h-screen fixed top-0 left-0 z-50 flex justify-center items-center' onClick={() => set_show_modal(false)}>

              <div className='w-10/12 mx-auto md:max-w-sm space-y-2' onClick={e => e.stopPropagation()}>
                <p className='text-white'>{btn_action == PRINT ? "Enter text to name generated pdf" : "Enter text to save result as"} </p>
                <input className='bg-white p-2 w-full rounded-lg focus:outline-none focus:border-2 focus:bg-white  focus:border-[#6c43e8]' onChange={(e) => { set_pdf_name(e.target.value) }} onClick={e => e.stopPropagation()}></input>
                <button disabled={!pdf_name.length} className='bg-[#6c43e8] rounded-md w-full p-3 text-white hover:opacity-90' onClick={(e) => {
                  e.stopPropagation();
                  if (!pdf_name.match(PDF_NAME_REGEX)) {
                    alert("Please enter a valid name!")
                    return;
                  }
                  if (btn_action == SAVE) {
                    perform_save(input_value, test_info, dashboard_info, set_is_saving_result, set_show_modal, set_input_value)
                  }
                  if (btn_action == PRINT) {

                    saveAsPDF(test_info, dashboard_info["test_type"], dashboard_info["sensor_rating_kg"], dashboard_info["target_force"], dashboard_info["target_extension"], pdf_name)
                    set_pdf_name("")
                    set_show_modal(false)
                  }
                }}> {btn_action == PRINT ? "Proceed to print" : (is_saving_result ? "saving..." : "Proceed to save")}</button>
              </div>
            </div> : null
        }


        {

          <section className=''>

            <section className='flex flex-col md:flex-row  gap-7  px-2 '>
              <div className='md:fixed md:top-1/2 md:-translate-y-1/2 justify-self-start md:w-2/12  flex-1 flex flex-col-reverse md:flex-col items-start gap-8'>

                <Sidebar dashboard_info={dashboard_info} status={status} />
              </div>
              <div className='w-full md:w-8/12 md:mx-auto   gap-6 md:gap-8  ' id={"content-id"}>
                {/* remove mockData */}
                <div className='flex flex-col mb-8 md:flex-row md:gap-4 justify-evenly  '>
                  <DataCard title="Force Reading (N)" value={test_info[test_info.length - 1]?.force_in_newton || "-----"} valueSize={"bigger"} className={"flex-1"} theme={"blueBg"} />
                  <DataCard title={dashboard_info["test_type"] == "torsion" ? "Angular displacement(°) " : "Displacement(mm)"} value={test_info[test_info.length - 1]?.displacement_in_mm || "----"} valueSize={"bigger"} className={"flex-1"} theme={"blueBg"} />

                </div>
                <div className='h-[50vh] flex-1 gap-6  flex-shrink-0 w-full md:h-[70vh] flex flex-col md:justify-between  overflow-x-scroll md:overflow-x-auto'  >
                  <div className=' w-full h-[80%]  md:h-full  ' id='graph' >

                    <Graph data={test_info} test_type={dashboard_info["test_type"]} />

                  </div>

                  {

                    !!test_info?.length ?


                      <div className={`flex gap-4 justify-end ${show_modal ? "hidden" : "block"}`}>
                        <button className='px-5 py-2 hover:bg-black text-black hover:text-white rounded-xl text-sm hover:opacity-90 hover:scale-95 border-2 border-black' onClick={() => {
                          // set_show_modal(true)
                          // set_btn_action(PRINT)
                          set_btn_action(PRINT)
                          set_show_modal(true)
                        }
                        }>Print Result</button>

                      </div> : null
                  }



                </div>
              </div>
              <div className='md:fixed  right-0  w-full md:w-2/12  flex-1 flex flex-col-reverse md:flex-col items-start gap-8'>
                <Table test_info={test_info} dashboard_info={dashboard_info} />

              </div>
            </section>
          </section>
        }


      </div >
    </section>

  );
};

export default Dashboard;
