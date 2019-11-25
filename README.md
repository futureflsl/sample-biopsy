EN|[CN](README_cn.md)

# Biopsy<a name="EN-US_TOPIC_0167573069"></a>

Developers can deploy the application on the Atlas 200 DK to predict the biopsy in the video by using the camera.

## Prerequisites<a name="en-us_topic_0182554631_section137245294533"></a>

Before using an open source application, ensure that:

-   Mind Studio  has been installed.
-   The Atlas 200 DK developer board has been connected to  Mind Studio, the cross compiler has been installed, the SD card has been prepared, and basic information has been configured.

## Software Preparation<a name="en-us_topic_0182554631_section8534138124114"></a>

Before running the application, obtain the source code package and configure the environment as follows.

1.  <a name="en-us_topic_0182554631_li953280133816"></a>Obtain the source code package.

    Download all the code in the sample-facialrecognition repository at  [https://github.com/Ascend/sample-biopsy](https://github.com/Ascend/sample-biopsy)  to any directory on Ubuntu Server where  Mind Studio  is located as the  Mind Studio  installation user, for example,  _/home/ascend/sample-biopsy.

2.  Obtain the source network model required by the application.

    Obtain the source network model and its weight file used in the application by referring to  [Table 1](#en-us_topic_0182554631_table97791025517), and save them to any directory on the Ubuntu server where  Mind Studio  is located (for example,  **$HOME/ascend/models/depl**).

    **Table  1**  Models used for biopsy

    <a name="en-us_topic_0182554631_table97791025517"></a>
    <table><thead align="left"><tr id="en-us_topic_0182554631_row48791253115"><th class="cellrowborder" valign="top" width="13.309999999999999%" id="mcps1.2.4.1.1"><p id="en-us_topic_0182554631_p187902511114"><a name="en-us_topic_0182554631_p187902511114"></a><a name="en-us_topic_0182554631_p187902511114"></a>Model Name</p>
    </th>
    <th class="cellrowborder" valign="top" width="12.04%" id="mcps1.2.4.1.2"><p id="en-us_topic_0182554631_p148791259118"><a name="en-us_topic_0182554631_p148791259118"></a><a name="en-us_topic_0182554631_p148791259118"></a>Model Description</p>
    </th>
    <th class="cellrowborder" valign="top" width="74.65%" id="mcps1.2.4.1.3"><p id="en-us_topic_0182554631_p987922511111"><a name="en-us_topic_0182554631_p987922511111"></a><a name="en-us_topic_0182554631_p987922511111"></a>Model Download Path</p>
    </th>
    </tr>
    </thead>
    <tbody><tr id="en-us_topic_0182554631_row38791825912"><td class="cellrowborder" valign="top" width="13.309999999999999%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0182554631_p0879152519115"><a name="en-us_topic_0182554631_p0879152519115"></a><a name="en-us_topic_0182554631_p0879152519115"></a>face_detection</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.04%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0182554631_p52941556525"><a name="en-us_topic_0182554631_p52941556525"></a><a name="en-us_topic_0182554631_p52941556525"></a>Network model for face detection.</p>
    <p id="en-us_topic_0182554631_p13913132012525"><a name="en-us_topic_0182554631_p13913132012525"></a><a name="en-us_topic_0182554631_p13913132012525"></a>It is a network model converted from ResNet0-SSD300 model based on Caffe.</p>
    </td>
    <td class="cellrowborder" valign="top" width="74.65%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0182554631_p188801525813"><a name="en-us_topic_0182554631_p188801525813"></a><a name="en-us_topic_0182554631_p188801525813"></a>Download the source network model file and its weight file by referring to<strong id="en-us_topic_0182554631_b6722175014127"><a name="en-us_topic_0182554631_b6722175014127"></a><a name="en-us_topic_0182554631_b6722175014127"></a> README.md</strong> in <a href="https://github.com/Ascend/models/tree/master/computer_vision/object_detect/face_detection" target="_blank" rel="noopener noreferrer">https://github.com/Ascend/models/tree/master/computer_vision/object_detect/depl</a>.</p>
    </td>
    </tr>
    <tr id="en-us_topic_0182554631_row11880162511114"><td class="cellrowborder" valign="top" width="13.309999999999999%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0182554631_p1388012251117"><a name="en-us_topic_0182554631_p1388012251117"></a><a name="en-us_topic_0182554631_p1388012251117"></a>biopsy</p>
    </td>
    <td class="cellrowborder" valign="top" width="12.04%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0182554631_p1988018251110"><a name="en-us_topic_0182554631_p1988018251110"></a><a name="en-us_topic_0182554631_p1988018251110"></a>Network model for predicting facial expression.</p>
    <p id="en-us_topic_0182554631_p1057942195213"><a name="en-us_topic_0182554631_p1057942195213"></a><a name="en-us_topic_0182554631_p1057942195213"></a>It is a network model converted from the biopsy model based on Caffe.</p>
    </td>
    <td class="cellrowborder" valign="top" width="74.65%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0182554631_p28801025319"><a name="en-us_topic_0182554631_p28801025319"></a><a name="en-us_topic_0182554631_p28801025319"></a>Download the source network model file and its weight file by referring to<strong id="en-us_topic_0182554631_b47241650201210"><a name="en-us_topic_0182554631_b47241650201210"></a><a name="en-us_topic_0182554631_b47241650201210"></a> README.md</strong> in <a href="https://github.com/Ascend/models/tree/master/computer_vision/classification/vanillacnn" target="_blank" rel="noopener noreferrer">https://github.com/Ascend/models/tree/master/computer_vision/classification/depl</a>.</p>
    </td>
    </tr>
    </tbody>
    </table>

3.  Convert the source network model to a Da Vinci model.
    1.  Choose  **Tool \> Convert Model**  from the main menu of  Mind Studio. The  **Convert Model**  page is displayed.
    2.  On the  **Convert Model**  page, set** Model File**  and  **Weight File**  to the model file and weight file downloaded in Step 2, respectively.
        -   Set  **Model Name**  to the model name in  [Table 1](#en-us_topic_0182554631_table97791025517).
        -   Configure model conversion for the facedetection and biopsy models by referring to  [Figure 1](#en-us_topic_0182554631_fig1513227955)  and  [Figure 2](#en-us_topic_0182554631_fig61342716510), respectively.

            **Figure  1**  VanillaCNNModel Model Conversion Configuration Reference<a name="en-us_topic_0182554631_fig1513227955"></a>  
             ![image](https://github.com/futureflsl/sample-biopsy/blob/master/images/face_detection.png)

            
            **Figure  2**  SpherefaceModel Model Conversion Configuration Reference<a name="en-us_topic_0182554631_fig61342716510"></a>  
           ![image](https://github.com/futureflsl/sample-biopsy/blob/master/images/depl.png)           

    3.  Click  **OK**  to start model conversion.

        During the conversion of the face\_detection model, the following error will be reported.

        **Figure  3**  Model conversion error<a name="en-us_topic_0182554631_fig1632884495219"></a>  
        ![image](https://github.com/futureflsl/sample-biopsy/blob/master/images/model-conversion-error.jpg)

        Select  **SSDDetectionOutput**  from the  **Suggestion**  drop-down list box at the  **DetectionOutput**  layer and click  **Retry**.

        After successful conversion, a .om Da Vinci model is generated in the  **$HOME/tools/che/model-zoo/my-model/xxx**  directory.

4.  Upload the converted .om model file to the  **sample-biopsy/script** directory in the source code path in  [1](#en-us_topic_0182554631_li953280133816).
5.  Log in to Ubuntu Server where  Mind Studio  is located as the  Mind Studio  installation user and set the environment variable  **DDK\_HOME**.

    **vim \~/.bashrc**

    Run the following commands to add the environment variables  **DDK\_HOME**  and  **LD\_LIBRARY\_PATH**  to the last line:

    **export DDK\_HOME=/home/XXX/tools/che/ddk/ddk**

    **export LD\_LIBRARY\_PATH=$DDK\_HOME/uihost/lib**

    >![](doc/source/img/icon-note.gif) **NOTE:**   
    >-   **XXX**  indicates the  Mind Studio  installation user, and  **/home/XXX/tools**  indicates the default installation path of the DDK.  
    >-   If the environment variables have been added, skip this step.  

    Enter  **:wq!**  to save and exit.

    Run the following command for the environment variable to take effect:

    **source \~/.bashrc**


## Deployment<a name="en-us_topic_0182554631_section147911829155918"></a>

1.  Access the root directory where the biopsy application code is located as the  Mind Studio  installation user, for example,  **_/home/ascend/sample-biopsy**.
2.  <a name="en-us_topic_0182554631_li08019112542"></a>Run the deployment script to prepare the project environment, including compiling and deploying the ascenddk public library, and configuring Presenter Server. The Presenter Server is used to receive the data sent by the application and display the result through the browser.

    **bash deploy.sh** _host\_ip_ _model\_mode_

    -   _host\_ip_: this parameter indicates the IP address of the Atlas 200 DK developer board.

    -   _model\_mode_  indicates the deployment mode of the model file. The default setting is  **internet**.
        -   **local**: If the Ubuntu system where  Mind Studio  is located is not connected to the network, use the local mode. In this case, download the dependent common code library to the  **/sample-biopsy/script**  directory, by referring to  [Downloading Dependent Code Library](#en-us_topic_0182554631_section158977311307).
        -   **internet**: If the Ubuntu system where  Mind Studio  is located is connected to the network, use the Internet mode. In this case, download the dependent code library online.


    Example command:

    **bash deploy.sh 192.168.1.2 internet**

    -   When the message  **Please choose one to show the presenter in browser\(default: 127.0.0.1\):**  is displayed, enter the IP address used for accessing the Presenter Server service in the browser. Generally, the IP address is the IP address for accessing the  Mind Studio  service.
   
    Select the IP address used by the browser to access the Presenter Server service in  **Current environment valid ip list**  and enter the path for storing facial recognition data, as shown in  [Figure 4](#en-us_topic_0182554631_fig184321447181017).

    **Figure  4**  Project deployment<a name="en-us_topic_0182554631_fig184321447181017"></a>  
    ![image](https://github.com/futureflsl/sample-biopsy/blob/master/images/%E5%B7%A5%E7%A8%8B%E9%83%A8%E7%BD%B2%E7%A4%BA%E6%84%8F%E5%9B%BE.png)

3.  Start Presenter Server.

    Run the following command to start the Presenter Server program of the facial recognition application in the background:

    **bash start_presenterserver.sh**

    **Figure  5**  Starting the Presenter Server process<a name="en-us_topic_0182554631_fig69531305324"></a>  
    ![images](https://github.com/futureflsl/sample-biopsy/blob/master/images/Presenter-Server%E8%BF%9B%E7%A8%8B%E5%90%AF%E5%8A%A8.png)

    Use the URL shown in the preceding figure to log in to Presenter Server \(only the Chrome browser is supported\). The IP address is that entered in  [2](#en-us_topic_0182554631_li08019112542)  and the default port number is  **7009**. The following figure indicates that Presenter Server is started successfully.

    **Figure  6**  Home page<a name="en-us_topic_0182554631_fig64391558352"></a>  
   ![images](https://github.com/futureflsl/sample-biopsy/blob/master/images/Presenter-Server%E8%BF%9B%E7%A8%8B%E5%90%AF%E5%8A%A8.png)

    The following figure shows the IP address used by the Presenter Server and  Mind Studio  to communicate with the Atlas 200 DK.

    **Figure  7**  Example IP Address<a name="en-us_topic_0182554631_fig14929132312013"></a>  
    ![images](https://github.com/futureflsl/sample-biopsy/blob/master/images/IP%E5%9C%B0%E5%9D%80%E7%A4%BA%E4%BE%8B.png)

    Where:

    -   The IP address of the Atlas 200 DK developer board is 192.168.1.2 \(connected in USB mode\).
    -   The IP address used by the Presenter Server to communicate with the Atlas 200 DK is in the same network segment as the IP address of the Atlas 200 DK on the UI Host server. For example: 192.168.1.223.
    -   The following is an example of accessing the IP address of the Presenter Server using a browser: 10.10.0.1, because the Presenter Server and  Mind Studio  are deployed on the same server, the IP address is also the IP address for accessing the  Mind Studio  through the browser.


## Running<a name="en-us_topic_0182554631_section1676879104"></a>

1.  Run the Biopsy application.

    Run the following command in the  **sample-biopsy**  directory to start the facial recognition application:

    **bash run\_biopsyapp.sh** _host\_ip_ _presenter\_view\_app\_name camera\_channel\_name_  &

    -   _host\_ip_: For the Atlas 200 DK developer board, this parameter indicates the IP address of the developer board.
    -   _presenter\_view\_app\_name_: Indicates  **App Name**  displayed on the Presenter Server page, which is user-defined. The value of this parameter must be unique on the Presenter Server page, which contains only case-senstive leters, digits, and underscores(_). The number of characters should be 3-20.
    -   _camera\_channel\_name_: Indicates the channel to which a camera belongs. The value can be  **Channel-1**  or  **Channel-2**.

        For details, see  **View the Channel to Which a Camera Belongs**  in  [Atlas 200 DK User Guide](https://ascend.huawei.com/documentation).

    -   Example command:

    **bash run\_biopsyapp.sh 192.168.1.2 video Channel-1 &**

2.  Use the URL that is displayed when you start the Presenter Server service to log in to the Presenter Server website \(only the Chrome browser is supported\).

    [Figure 8](#en-us_topic_0182554631_fig1189774382115)  shows the Presenter Server page.

    **Figure  8**  Presenter Server page<a name="en-us_topic_0182554631_fig1189774382115"></a>  
    ![image](https://github.com/futureflsl/sample-biopsy/blob/master/images/Presenter-Server%E7%95%8C%E9%9D%A2.png)

    >![](doc/source/img/icon-note.gif) **NOTE:**   
    >-   The Presenter Server of the facial recognition application supports a maximum of two channels at the same time (each  _presenter\_view\_app\_name_  corresponds to a channel).  
    >-   Due to hardware limitations, the maximum frame rate supported by each channel is 20fps, a lower frame rate is automatically used when the network bandwidth is low.  

## Follow-up Operations<a name="en-us_topic_0182554631_section1092612277429"></a>

-   **Stopping the Biopsy Application**

    The facial recognition application is running continually after being executed. To stop it, perform the following operation:

    Run the following command in the  **sample-biopsy**  directory as the  Mind Studio  installation user:

    **bash stop\_biopsyapp.sh** _host\_ip_

    _host\_ip_: For the Atlas 200 DK developer board, this parameter indicates the IP address of the developer board.

    Example command:

    **bash stop\_biopsyapp.sh 192.168.1.2**

-   **Stopping the Presenter Server Service**

    The Presenter Server service is always in the running state after being started. To stop the Presenter Server service of the facial recognition application, perform the following operations:

    **bash stop_presenterserver.sh**

## Downloading Dependent Code Library<a name="en-us_topic_0182554631_section158977311307"></a>

Download the dependent software libraries to the  **/sample-biopsy/script**  directory.

**Table  2**  Download the dependent software library

<a name="en-us_topic_0182554631_table915515518188"></a>
<table><thead align="left"><tr id="en-us_topic_0182554631_row1778815582332"><th class="cellrowborder" valign="top" width="33.373337333733375%" id="mcps1.2.4.1.1"><p id="en-us_topic_0182554631_p1278885843311"><a name="en-us_topic_0182554631_p1278885843311"></a><a name="en-us_topic_0182554631_p1278885843311"></a>Module Name</p>
</th>
<th class="cellrowborder" valign="top" width="33.29332933293329%" id="mcps1.2.4.1.2"><p id="en-us_topic_0182554631_p1378815833318"><a name="en-us_topic_0182554631_p1378815833318"></a><a name="en-us_topic_0182554631_p1378815833318"></a>Module Description</p>
</th>
<th class="cellrowborder" valign="top" width="33.33333333333333%" id="mcps1.2.4.1.3"><p id="en-us_topic_0182554631_p1778895893314"><a name="en-us_topic_0182554631_p1778895893314"></a><a name="en-us_topic_0182554631_p1778895893314"></a>Download Address</p>
</th>
</tr>
</thead>
<tbody><tr id="en-us_topic_0182554631_row478815581332"><td class="cellrowborder" valign="top" width="33.373337333733375%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0182554631_p878895812336"><a name="en-us_topic_0182554631_p878895812336"></a><a name="en-us_topic_0182554631_p878895812336"></a>EZDVPP</p>
</td>
<td class="cellrowborder" valign="top" width="33.29332933293329%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0182554631_p478885818334"><a name="en-us_topic_0182554631_p478885818334"></a><a name="en-us_topic_0182554631_p478885818334"></a>Encapsulates the DVPP interface and provides image and video processing capabilities, such as color gamut conversion and image / video conversion</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0182554631_p1078865873316"><a name="en-us_topic_0182554631_p1078865873316"></a><a name="en-us_topic_0182554631_p1078865873316"></a><a href="https://github.com/Ascend/sdk-ezdvpp" target="_blank" rel="noopener noreferrer">https://github.com/Ascend/sdk-ezdvpp</a></p>
<p id="en-us_topic_0182554631_p37881158143319"><a name="en-us_topic_0182554631_p37881158143319"></a><a name="en-us_topic_0182554631_p37881158143319"></a>After the download, keep the folder name <span class="filepath" id="en-us_topic_0182554631_filepath147883587339"><a name="en-us_topic_0182554631_filepath147883587339"></a><a name="en-us_topic_0182554631_filepath147883587339"></a><b>ezdvpp</b></span>.</p>
</td>
</tr>
<tr id="en-us_topic_0182554631_row17788558153315"><td class="cellrowborder" valign="top" width="33.373337333733375%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0182554631_p9788135810331"><a name="en-us_topic_0182554631_p9788135810331"></a><a name="en-us_topic_0182554631_p9788135810331"></a>Presenter Agent</p>
</td>
<td class="cellrowborder" valign="top" width="33.29332933293329%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0182554631_p137881858113312"><a name="en-us_topic_0182554631_p137881858113312"></a><a name="en-us_topic_0182554631_p137881858113312"></a><span>API for interacting with the Presenter Server</span>.</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0182554631_p134402020123313"><a name="en-us_topic_0182554631_p134402020123313"></a><a name="en-us_topic_0182554631_p134402020123313"></a><a href="https://github.com/Ascend/sdk-presenter/tree/master" target="_blank" rel="noopener noreferrer">https://github.com/Ascend/sdk-presenter/tree/master</a></p>
<p id="en-us_topic_0182554631_p5440152033310"><a name="en-us_topic_0182554631_p5440152033310"></a><a name="en-us_topic_0182554631_p5440152033310"></a>Obtain the presenteragent folder in this path, after the download, keep the folder name <span class="filepath" id="en-us_topic_0182554631_filepath1440192033318"><a name="en-us_topic_0182554631_filepath1440192033318"></a><a name="en-us_topic_0182554631_filepath1440192033318"></a><b>presenteragent</b></span>.</p>
</td>
</tr>
<tr id="en-us_topic_0182554631_row97890586339"><td class="cellrowborder" valign="top" width="33.373337333733375%" headers="mcps1.2.4.1.1 "><p id="en-us_topic_0182554631_p4789115820333"><a name="en-us_topic_0182554631_p4789115820333"></a><a name="en-us_topic_0182554631_p4789115820333"></a>tornado (5.1.0)</p>
<p id="en-us_topic_0182554631_p578945843318"><a name="en-us_topic_0182554631_p578945843318"></a><a name="en-us_topic_0182554631_p578945843318"></a>protobuf (3.5.1)</p>
<p id="en-us_topic_0182554631_p1878925843318"><a name="en-us_topic_0182554631_p1878925843318"></a><a name="en-us_topic_0182554631_p1878925843318"></a>numpy (1.14.2)</p>
</td>
<td class="cellrowborder" valign="top" width="33.29332933293329%" headers="mcps1.2.4.1.2 "><p id="en-us_topic_0182554631_p6789258143315"><a name="en-us_topic_0182554631_p6789258143315"></a><a name="en-us_topic_0182554631_p6789258143315"></a>Python libraries that Presenter Server depends on.</p>
</td>
<td class="cellrowborder" valign="top" width="33.33333333333333%" headers="mcps1.2.4.1.3 "><p id="en-us_topic_0182554631_p156393307316"><a name="en-us_topic_0182554631_p156393307316"></a><a name="en-us_topic_0182554631_p156393307316"></a>You can search for related packages on the Python official website <a href="https://pypi.org/" target="_blank" rel="noopener noreferrer">https://pypi.org/</a> for installation. If you run the pip3 install command to download the file online, you can run the following command to specify the version to be downloaded: <strong id="en-us_topic_0182554631_b84911294419"><a name="en-us_topic_0182554631_b84911294419"></a><a name="en-us_topic_0182554631_b84911294419"></a>pip3 install tornado==5.1.0 -i <em id="en-us_topic_0182554631_i1556317151418"><a name="en-us_topic_0182554631_i1556317151418"></a><a name="en-us_topic_0182554631_i1556317151418"></a>Installation source of the specified library</em> --trusted-host <em id="en-us_topic_0182554631_i9475221741"><a name="en-us_topic_0182554631_i9475221741"></a><a name="en-us_topic_0182554631_i9475221741"></a>Host name of the installation sourc</em>e</strong></p>
</td>
</tr>
</tbody>
</table>

